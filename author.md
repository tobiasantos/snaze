# Identificação Pessoal

Preencha os dados abaixo para identificar a autoria do trabalho.

- Nome: Tobias dos Santos Neto
- Email: tobias.neto.106@ufrn.edu.br
- Turma: DIM0119 - T01

- Nome: Wisla Alves Argolo
- Email: wisla.argolo.133@ufrn.edu.br
- Turma: DIM0119 - T01

# Indique quais itens da avaliação foram realizados

- [x] Valida corretamente arquivo de entrada (20 pontos)

- [x] Simulador apenas sugere direções randômicas para cobra (10 pontos)
OU
- [x] Simulador direciona a cobra corretamente para a comida (50 pontos)

- [x] Apresentar e manipula corretamente os estados da simulação (30 pontos)

- [x] **Extra**: simulação gera o menor caminho
- [ ] **Extra**: 

# Compilação 

```
cmake -S source -B build
cmake --build build
```

## Executando

```
./build/snaze ./assets/input_level_file [options]
```
### Configurações

Game simulation options:
```
--help                  # Print this help text.
--fps <num>             # Number of frames (board) presented per second. Default = 12. Valid range = [1, 50]
--lives <num>           # Number of lives the snake shall have. Default = 5. Valid range = [1, 20]
--food <num>            # Number of food pellets for the entire simulation. Default = 10. Valid range = [1, 20]
--playertype <type>     # Type of snake intelligence: random, backtracking. Default = backtracking
```

# Limitações

É preciso ter as seguintes fontes instaladas para os ícones utilizados na simulação: 
 
 - Phosphor (https://phosphoricons.com) 
 - 3270 Nerd Font (https://www.nerdfonts.com)
   
Caso contrário, é necessário modificar os símbolos utilizados no render.

--------
&copy; DIMAp/UFRN 2021.
