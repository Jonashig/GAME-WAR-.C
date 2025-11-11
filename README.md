# GAME-WAR-.C
# 🌍 Jogo WAR em C

Simulador de estratégia inspirado no clássico jogo War, desenvolvido em linguagem C para terminal.  
O jogador gerencia territórios, realiza ataques entre exércitos e tenta conquistar o mapa.

---

## 🧠 Funcionalidades
- Cadastro dinâmico de territórios (usando malloc/calloc)
- Exibição completa do mapa-múndi
- Sistema de turnos
- Simulação de ataques com dados aleatórios (`rand()`)
- Atualização automática de tropas e conquistas
- Organização modular em funções
- Liberação de memória com `free()`

---

## ⚔️ Como jogar
1. Informe os dados dos territórios (nome, cor, tropas).
2. O jogo entra em modo de turnos.
3. Escolha o território atacante e o defensor.
4. Os dados são lançados automaticamente e o resultado decide o vencedor.
5. Quem vence ganha uma tropa; quem perde, perde uma.

---

## 💾 Execução
Compile o código no terminal:
```bash
gcc war.c -o war
./war
