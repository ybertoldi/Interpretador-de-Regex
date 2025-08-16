# Interpretador de Regex
Projeto baseado nos artigos de [Ken Thompson](https://dl.acm.org/doi/pdf/10.1145/363347.363387) e [Russ Cox](https://swtch.com/~rsc/regexp/regexp1.html).

Trata-se de um programa que converte uma expressão regular em AFND segiundo três passos simples:

- Transformar a expressão regular recebida em [notação polonesa inversa](https://pt.wikipedia.org/wiki/Nota%C3%A7%C3%A3o_polonesa_inversa) (ou postfix), evidenciando as operações
  de concatenação e retirando os parenteses.

  Por exemplo:

  |    Regex     |   Notação Polonesa Inversa   |
  | ------------ | -----------------------------|
  |     aac      |            aa.c.             |
  |   ab(b\|d)\*   |           ab.bd\|\*.           |
  |   a\*(b\|c)+   |           a*bc\|+.            |

  A implementação deste passo está em [regex2post.c](includes/regex2post.c).




- Interpretar o texto utilizando os algoritmos de conversão de Thompson.
  
  Operações feitas com a notação polonesa inversa são facilmente interpretadas utilizando uma pilha.
  
  Basta guardar guardar valores (no caso AFNDs) ao serem lidos e retira-los para realizar cada uma de suas operações.    Uma implementação de calculadora para a notação polonesa inversa é encontrada em *The C Programming Language, 2 ed.*   no capítulo 4.3, onde é descrito o seguinte pseudo-código: ![image](https://github.com/user-attachments/assets/ad04a279-dca5-4347-b0ce-3562b6a0e97e)

  A implementação desse passo é feita em [main.c](main.c)




- Navegar o AFND gerado.
 
  Esse passo é feito ao fim de [main.c](main.c) e seu algoritmo está implementado nas funções nfa_valida_cadeia e nfa_teste em [nfa_node.c](includes/nfa_node.c). Trata-se de uma variação de um algoritmo BFS padrão.

## Como Rodar
clone o repositório e execute o script BUILD.sh:
```bash
$ bash BUILD.sh
$ cd execs
```
Serão gerados 4 executaveis, sendo 3 desses exemplos das funcionalidades dos headers e o último uma implementação do 
algoritmo mencionado acima.



