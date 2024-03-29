# Projeto PDS2

Implementacao simples de um chat usando o protocolo de comunicação IRC (Internet Relay Chat)

# How to use
Para utilizar o chat basta seguir os passos abaixo na respectiva ordem:
* Compilar o programa. Para isso, basta entrar na pasta principal e executar o código abaixo
    
      make

* Habilitar o server, executando o código abaixo dentro da pasta '/server/build':
    
      ./server <NUMERO_DA_PORTA>

* Criar quantos clientes desejar, executando o código abaixo dentro da pasta '/client/build':
    
      ./client <IP_SERVER> <NUMERO_DA_PORTA> <NICKNAME> <CHANNEL_NAME>
      
Obs: para ip local, basta usar 127.0.0.1

Obs 2: você deve rodar o código acima uma vez para cada cliente

# Workflow

Adotaremos o seguinte processo para desenvolvimento do projeto:

* Escolha uma funcionalidade, alteração, correção de bug para resolver (estarão no Trello)

* Crie um novo branch à partir do master com o comando:*
    
      git checkout -b <NOME_DO_BRANCH>
                
* Codifique a nova funcionalidade / alteração no branch criado com os comandos:

      git commit -m <MENSAGEM_COMMIT>
      git push
                
* Ao desenvolver certifique-se de testar todas as funcionalidades adicionadas

* Crie um Pull Request para review e aprovação do seu código, este deverá ser avaliado por pelo menos um integrante do grupo antes de dar um merge no master, portanto insira comentários q deixe-o claro para os outros desenvolvedores

# Trello do projeto:

https://trello.com/b/LkxBD7j1/projeto-pds2
