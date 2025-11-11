#!/usr/bin/env python

import _thread
import argparse
import os
import re
import sys
import threading
from contextlib import contextmanager
from os import listdir
from os.path import isfile, join
from subprocess import PIPE, TimeoutExpired, run
from typing import List, Set, Tuple, Dict, FrozenSet
from dataclasses import dataclass

class BaseTask:
    TIME_LIMIT_SECONDS = 2
    ABS_TOLERANCE = 0.001

    SPACES_RE = re.compile(r"\s+", re.M)
    CFLAGS = ["-std=c99", "-Wall", "-Werror", "-Wvla", "-g"]
    LDFLAGS = ["-lm"]

    def __init__(self, continue_on_error=True, only_matching=None):
        self.continue_on_error = continue_on_error
        self.only_matching = only_matching
        self.compiled_binaries = set()
        self.tested_cases = set()
        self.passed_cases = set()
        self.tested_units = set()
        self.passed_units = set()
        self.number_cases = 0
        self.run_valgrind = False
        self.show_all_errors = False
        self.python3 = self.find_python3()
        self.runner_ra = None

    def find_python3(self):
        suffix = ".exe" if os.name == "nt" else ""
        for py in ["python3", "python", "py"]:
            for path in os.environ["PATH"].split(os.pathsep):
                exe_file = os.path.join(path, py + suffix)
                if isfile(exe_file):
                    return py
        print("o interpretador python3 não está no path; verifique sua instalação")
        sys.exit(1)

    def read_file_utf8(self, filename):
        assert isfile(filename), f"não existe o arquivo {filename}"
        try:
            with open(filename, encoding="utf-8", errors="strict") as f:
                return f.read()
        except ValueError:
            raise AssertionError(f"enconding inválido em {filename}; por favor, use UTF-8")
        except Exception as e:  # noqa
            raise AssertionError(f"falha ao ler arquivo {filename}: {e}")

    def strip_spaces(self, text):
        return self.SPACES_RE.sub(" ", text.strip())

    def compare_stripped(self, out, res):
        return self.strip_spaces(out) == self.strip_spaces(res)

    def exists(self, filename):
        assert isfile(filename), f"você deve criar um arquivo {filename}"

    def normalize_binary_name(self, binary):
        if os.name == "nt":
            binary += ".exe"
        return join(".", binary)

    def compare_files(self, out, res):
        out = self.read_file_utf8(out)
        res = self.read_file_utf8(res)
        return self.compare_output(out, res)

    @staticmethod
    def get_possible_number(text):
        try:
            return float(text)
        except ValueError:
            return None

    def compare_doubles(self, out, res):
        import math

        out = out.split()
        res = res.split()
        if len(out) != len(res):
            return False
        for a, b in zip(out, res):
            if a == b:
                continue
            a = self.get_possible_number(a)
            b = self.get_possible_number(b)
            if a is None or b is None:
                return False
            if not math.isclose(a, b, abs_tol=self.ABS_TOLERANCE):
                return False
        return True

    def compare_output(self, out, res):
        return self.compare_stripped(out, res)

    def check_makefile(self):
        self.exists("Makefile")
        with open("Makefile") as fp:
            cflags_lines = [s for s in fp.readlines() if s.startswith("CFLAGS")]
        assert len(cflags_lines) == 1, "deve haver uma linha CFLAGS no Makefile"
        cflags = set(cflags_lines[0].split())
        assert cflags.issuperset(self.CFLAGS), f"adicione as flags: {' '.join(self.CFLAGS)}"

    def make(self, binary):
        norm_binary = self.normalize_binary_name(binary)
        if binary not in self.compiled_binaries:
            if isfile(norm_binary):
                os.remove(norm_binary)
            self.compiled_binaries.add(binary)
            self.check_makefile()
            p = run(["make", binary])
            assert p.returncode == 0, f"falha ao criar {norm_binary} com Makefile"
        assert isfile(norm_binary), f"o Makefile não criou um arquivo {norm_binary}"
        return norm_binary

    def compile(self, sources=None, binary=None):
        if sources is None:
            sources = sorted([f for f in listdir(".") if f.endswith(".c")])
        if binary is None:
            binary = sources[0][:-2]  # binário padrão é primeiro arquivo sem extensão .c
        norm_binary = self.normalize_binary_name(binary)
        if binary not in self.compiled_binaries:
            if isfile(norm_binary):
                os.remove(norm_binary)
            self.compiled_binaries.add(binary)
            for source in sources:
                self.exists(source)
            compile_cmd = ["gcc"] + self.CFLAGS + sources + ["-o", binary] + self.LDFLAGS
            p = run(compile_cmd)
            assert p.returncode == 0, f"falha ao compilar {norm_binary}"
        assert isfile(norm_binary), f"não foi possível compilar {norm_binary}"
        return norm_binary

    def run_binary_inner(self, cmd, stdin, stdout, input):
        if input is None:
            p = run(
                cmd,
                stdin=stdin,
                stdout=stdout,
                encoding="utf8",
                errors="ignore",
                timeout=self.TIME_LIMIT_SECONDS,
            )
        else:
            p = run(
                cmd,
                input=input,
                stdout=stdout,
                encoding="utf8",
                errors="ignore",
                timeout=self.TIME_LIMIT_SECONDS,
            )
        assert p.returncode == 0, f"código de saída é {p.returncode}"
        return p

    def run_binary(
        self,
        cmd,
        stdin,
        stdout,
        input=None,
        in_filename=None,
        out_filename=None,
    ):
        cmd_str = " ".join([c if " " not in c and c != "" else f'"{c}"' for c in cmd])
        if in_filename:
            cmd_str += f" < {in_filename}"
        if out_filename:
            cmd_str += f" > {out_filename}"
        if input:
            cmd_str += f' com entrada "{input}"'
        try:
            return self.run_binary_inner(cmd, stdin, stdout, input)
        except AssertionError as e:
            raise AssertionError(f"falha ao executar {cmd_str} : {e}")
        except TimeoutExpired:
            raise AssertionError(
                f"falha ao executar {cmd_str} : tempo limite de {self.TIME_LIMIT_SECONDS}s excedido"
            )

    def test_one_case(self, cmd, in_filename):
        out_filename = in_filename.replace(".in", ".out")
        res_filename = in_filename.replace(".in", ".res")
        with open(in_filename) as i, open(out_filename, "w") as o:
            self.run_binary(cmd, i, o, in_filename=in_filename, out_filename=out_filename)
        ok = self.compare_files(out_filename, res_filename)
        assert ok, f'execute: diff "{out_filename}" "{res_filename}"'

    def test_cases(self, binary, in_filenames, folder="testes", args=[]):
        self.exists(binary)
        cmd = [binary] + args
        assert type(in_filenames) != str, "erro no caso de teste, deveria ser lista de strings"
        errors = []
        for in_filename in in_filenames:
            in_filename = join(folder, in_filename)
            try:
                self.tested_cases.add(in_filename)
                self.test_one_case(cmd, in_filename)
                self.passed_cases.add(in_filename)
                print(f"  -> {in_filename} passou")
            except AssertionError as e:
                print(f"  -> {in_filename} falhou")
                errors.append(str(e))
                if not self.continue_on_error:
                    break
        assert not errors, "\n  -> ".join(errors)

    def input_output(self, binary, input, expected, args=[]):
        self.exists(binary)
        cmd = [binary] + args
        p = self.run_binary(cmd, None, PIPE, input=input)
        output = p.stdout.strip()
        ok = self.compare_output(output, expected)
        assert ok, f'para entrada "{input}", a saída é "{output}", mas era esperado "{expected}"'

    def should_test(self, name):
        if not name.startswith("teste_"):
            return False
        if not self.only_matching:
            return True
        for pattern in self.only_matching:
            if pattern in name:
                return True
        return False

    def test_units(self):
        for name in sorted(dir(self)):
            if not self.should_test(name):
                continue
            print()
            print(f"Executando {name}...")
            sys.stderr.flush()
            sys.stdout.flush()
            try:
                test = getattr(self, name)
                self.tested_units.add(name)
                test()
                self.passed_units.add(name)
                print(f"{name}: OK")
            except AssertionError as e:
                print(f"{name}: FALHOU")
                if "privado" not in name or self.show_all_errors:
                    print(f"  -> {e}\n")
                if not self.continue_on_error:
                    break

    def case_range(self, input_template, start, end):
        input_files = []
        for i in range(start, end + 1):
            input_files.append(input_template.format(i))
        self.number_cases += len(input_files)
        return input_files

    def has_n_words(self, filename, n_words):
        text = self.read_file_utf8(filename)
        length = len(text.split())
        assert length >= n_words, f"{filename} deve ter pelos menos {n_words} palavras"

    @staticmethod
    @contextmanager
    def time_limit(seconds, desc):
        timer = threading.Timer(seconds, lambda: _thread.interrupt_main())
        timer.start()
        try:
            yield
        except KeyboardInterrupt:
            raise AssertionError(f"{desc} falhou: tempo limite de {seconds}s excedido")
        finally:
            timer.cancel()

    def test_closure(self, closure, desc="comando"):
        import traceback

        with self.time_limit(self.TIME_LIMIT_SECONDS, desc):
            try:
                closure()
            except Exception:
                tb = traceback.format_exc()
                assert False, f"algum erro ocorreu durante a execução:\n{tb}"

    @staticmethod
    def ler_ras(linha, inicio):
        linha = linha.strip().lower()
        inicio = inicio.lower()
        assert linha.startswith(inicio), "dupla.txt: formato do arquivo inválido"
        lista = linha.split(":", maxsplit=1)[1].replace(",", " ").split()
        for ra in lista:
            assert re.match(r"\d{6}$", ra), f"número de ra {ra} inválido"
            assert ra not in ["111111", "222222"], f"número de ra {ra} inválido"
        return lista

    def check_group_file(self):
        old_continue_on_error = self.continue_on_error
        self.continue_on_error = False

        self.exists("dupla.txt")
        with open("dupla.txt") as fp:
            elaborada = self.ler_ras(fp.readline(), "Tarefa elaborada pelos RAs:")
            submetida = self.ler_ras(fp.readline(), "Tarefa submetida apenas pelo RA:")
        assert len(submetida) == 1, "apenas um@ estudante deve submeter"
        assert len(elaborada) >= 1, "escreva pelo menos o seu número de RA"
        assert len(elaborada) <= 2, "atividade deve ser individual ou em dupla"
        assert set(elaborada).issuperset(
            submetida
        ), "RA de quem submeter deve estar na lista de em quem elaborou"
        submit_ra = submetida[0]

        if self.runner_ra is not None:
            print(
                "Conteúdo de dupla.txt:\n"
                + "\n".join(self.read_file_utf8("dupla.txt").split("\n")[:2])
                + "\n\n"
            )
            current_ra = self.runner_ra
        else:
            m = re.search(r"[/\\]ra([0-9]{6})[/\\]", os.path.abspath("."))
            current_ra = m.group(1) if m else None
        if current_ra is not None:
            assert (
                current_ra in elaborada
            ), "seu número de RA deve estar na lista de em quem elaborou"
        if current_ra != submit_ra:
            print("\n####################  ATENÇÃO  ####################\n")
            print(f"Tarefa configurada para ser submetida pelo RA {submit_ra}.")
            print("Códigos para essa tarefa presentes neste repositório")
            print("não serão considerados para a avaliação. Apenas a")
            print(f"submissão do RA {submit_ra} será considerada para a nota.")
            print("\n####################################################\n")

            if self.runner_ra is not None:
                raise AssertionError(f"correção será feita para estudante {submit_ra}")

        self.continue_on_error = old_continue_on_error


# --- Estrutura para o vértice e aresta (opcional, dependendo da complexidade do grafo)
@dataclass
class Vertice:
    id: int
    peso: int

@dataclass
class Aresta:
    u: int
    v: int

# --- Classe para representar o grafo
class Grafo:
    def __init__(self, n: int, pesos: List[int], arestas: List[Tuple[int, int]]):
        self.n = n
        self.pesos = pesos
        # Usando um dicionário de conjuntos para adjacência
        self.adj = {i: set() for i in range(n)}
        for u, v in arestas:
            if u != v: # Garante grafo simples
                self.adj[u].add(v)
                self.adj[v].add(u)

    def eh_clique(self, vertices: List[int]) -> bool:
        """Verifica se um conjunto de vértices forma um clique."""
        if len(vertices) <= 1:
            return True
        for i in range(len(vertices)):
            for j in range(i + 1, len(vertices)):
                if vertices[j] not in self.adj[vertices[i]]:
                    return False
        return True

    def peso_clique(self, vertices: List[int]) -> int:
        """Calcula o peso total de um clique."""
        return sum(self.pesos[v] for v in vertices)


# --- Banco de dados interno de soluções ótimas ---
# Cada entrada é: "nome_arquivo_sem_extensao.in": {"peso": valor, "cliques": {frozenset(...), frozenset(...)}}
SOLUCOES_OTIMAS = {
    "pequeno1": {
        "peso": 70,
        "cliques": {frozenset([0, 1, 2, 3])}
    },
    "pequeno2": {
        "peso": 29,
        "cliques": {frozenset([3, 4])}
    },
    "pequeno3": { 
        "peso": 32,
        "cliques": {frozenset([0, 1, 2]), frozenset([1, 2, 3])}
    },
    "pequeno4": {
        "peso": 60,
        "cliques": {frozenset([0, 1, 2]), frozenset([3, 4, 5])}
    },
    #######################################################################
    "medio1": {
        "peso": 50,
        "cliques": {frozenset([1, 2, 3, 4, 5])}
    },
    "medio2": {
        "peso": 174,
        "cliques": {frozenset([1, 3, 11])}
    },
    "medio3": {
        "peso": 78,
        "cliques": {frozenset([4, 9, 11, 13, 15])}
    },
    "medio4": {
        "peso": 250,
        "cliques": {frozenset([0, 1, 2, 3, 4]), frozenset([5, 6, 7, 8, 9]), frozenset([10, 11, 12, 13, 14]), 
                    frozenset([15, 16, 17, 18, 19]),
                    frozenset([20, 21, 22, 23, 24])}
    },
    ########################################################################
    "grande1": {
        "peso": 500,
        "cliques": {frozenset([5, 6, 7, 8, 9]), frozenset([10, 11, 12, 13, 14]), frozenset([0, 1, 2, 3, 4])}
    },
    "grande2": {
        "peso": 320,
        "cliques": {frozenset([30, 31, 32, 33]), frozenset([34, 35, 36, 37]), frozenset([38, 39, 40, 41])}
    },
    "grande3": {
        "peso": 1031,
        "cliques": {frozenset([0, 1, 6, 9, 10, 11, 12, 13, 29, 32, 33, 35, 37, 40, 42, 47, 48, 49]), 
                    frozenset([0, 1, 6, 9, 10, 11, 12, 29, 32, 33, 35, 37, 40, 42, 46, 47, 48, 49])}
    },
    "grande4": {
        "peso": 600,
        "cliques": {frozenset([50, 51, 52, 53, 54, 55])}
    },
    ########################################################################
    "tempo1": {
        "peso": 600,
        "cliques": {frozenset([50, 51, 52, 53, 54, 55]), frozenset([56, 57, 58, 59, 60, 61]), frozenset([62, 63, 64, 65, 66, 67])}
    },
    "tempo2": {
        "peso": 210,
        "cliques": {frozenset([0, 1, 2, 3, 4, 5, 6]), frozenset([7, 8, 9, 10, 11, 12, 13]), frozenset([14, 15, 16, 17, 18, 19, 20])}
    },
}


# --- Classe principal da Tarefa (BaseTask é assumida como existente)
class Task(BaseTask):
    @staticmethod
    def ler_instancia(in_filename: str) -> Grafo:
        """Lê o grafo a partir do arquivo .in."""
        with open(in_filename) as fp:
            n, m = map(int, fp.readline().split())
            pesos = list(map(int, fp.readline().split()))
            arestas = []
            for _ in range(m):
                u, v = map(int, fp.readline().split())
                arestas.append((u, v))
        return Grafo(n, pesos, arestas)

    @staticmethod
    def ler_solucao_aluno(out_filename: str) -> Tuple[int, List[int]]:
        """Lê a saída do aluno (peso e clique)."""
        with open(out_filename) as fp:
            try:
                valor = int(fp.readline().strip())
                clique_str = fp.readline().strip()
                if clique_str:
                    clique = list(map(int, clique_str.split()))
                else:
                    clique = []
            except (ValueError, IndexError):
                raise AssertionError(f"{out_filename}: formato de saída inválido.")
        return valor, clique

    def compare_files(self, out_filename: str, res_filename: str):
        """
        Compara a saída do aluno com as soluções ótimas precomputadas.
        Agora, as soluções são lidas do dicionário SOLUCOES_OTIMAS.
        """
        in_filename = out_filename.replace(".out", ".in")
        base_name = os.path.basename(in_filename).replace(".in", "")

        # Ler instância e soluções do aluno
        grafo = self.ler_instancia(in_filename)
        peso_aluno, clique_aluno = self.ler_solucao_aluno(out_filename)

        # Ler soluções ótimas do banco de dados interno
        if base_name not in SOLUCOES_OTIMAS:
            raise AssertionError(f"{out_filename}: não há soluções ótimas registradas para '{base_name}'.")

        peso_otimo = SOLUCOES_OTIMAS[base_name]["peso"]
        solucoes_otimas = SOLUCOES_OTIMAS[base_name]["cliques"]

        # --- Verificações ---
        # 1. Peso reportado pelo aluno deve ser igual ao peso ótimo conhecido
        assert peso_aluno == peso_otimo, \
            f"{out_filename}: encontrou clique com valor {peso_aluno}, mas o valor máximo é {peso_otimo}"

        # 2. O clique reportado deve ser um clique válido no grafo
        if not grafo.eh_clique(clique_aluno):
            raise AssertionError(f"{out_filename}: o conjunto de vértices {clique_aluno} não é um clique válido.")

        # 3. O peso real do clique do aluno deve corresponder ao valor reportado
        peso_real = grafo.peso_clique(clique_aluno)
        assert peso_real == peso_aluno, \
            f"{out_filename}: o valor reportado é {peso_aluno}, mas o clique {clique_aluno} vale {peso_real}"

        # 4. O clique reportado deve estar na lista de soluções ótimas
        clique_ordenado = frozenset(clique_aluno) # frozenset é hashable e pode ser membro de set
        assert clique_ordenado in solucoes_otimas, \
            f"{out_filename}: o clique {clique_aluno} (ordenado: {sorted(clique_aluno)}) não é uma das soluções ótimas conhecidas."

        return True # Sucesso se todas as verificações passarem

    def teste_1_pequeno(self):
        cases = self.case_range("pequeno{}.in", 1, 4)
        binary = self.make("rede")
        self.test_cases(binary, cases)

    def teste_2_medio(self):
        cases = self.case_range("medio{}.in", 1, 4)
        binary = self.make("rede")
        self.test_cases(binary, cases)

    def teste_3_grande(self):
        cases = self.case_range("grande{}.in", 1, 4)
        binary = self.make("rede")
        self.test_cases(binary, cases)


    def teste_4_tempo(self):
        cases = self.case_range("tempo{}.in", 1, 2)
        binary = self.make("rede")
        self.test_cases(binary, cases)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Testa tarefa.")
    parser.add_argument("only", nargs="*", help="apenas unidades contendo essa string")
    parser.add_argument("-c", action="store_true", help="continua mesmo que anteriores falhem")
    args = parser.parse_args()
    Task(args.c, args.only).test_units()