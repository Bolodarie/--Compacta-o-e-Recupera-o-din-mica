# Compactacao-RecuperacaoDinamica

Trabalho de Faculdade - Professor Orlando Figueiredo - Estrutura de dados II

O objetivo do projeto é gerenciar um sistema de arquivos que gerência a locação de veículos de uma dada
empresa. O sistema armazena as seguintes informações:
 Código do Cliente (CodCli),
 Código do Veículo (CodVei),
 Nome do Cliente,
 Nome do Veículo,
 Número de Dias

A chave primária é composta pela composição “CodCli+CodVei”. Para simplificar, considere que um
cliente pode alugar uma única vez um determinado carro (i.e., CodCli+CodVei será sempre único). O
arquivo a ser criado deve ser binário de registros e campos de tamanho variável, com 1 byte no início do
registro como indicador de tamanho do registro, e com campos separados pelo caractere ‘|’.

Código do Cliente - 11 caracteres-  (fixo)
| Código do Veículo - 7 caracteres - (fixo)
| Nome do Cliente - 50 caracteres - (máximo)
| Nome do Veículo - 50 caracteres - (máximo)
| Número de Dias - int -(fixo ou caracteres)|

As seguintes funcionalidades deverão estar disponíveis:
1. Inserção
2. Pesquisa por chave primária, i.e., “CodCli+CodVel” (índice primário)
3. Carrega Arquivos (dependente da implementação)

A fim de facilitar os testes do projeto, serão fornecidos dois arquivos:
a) “insere.bin”
b) “busca_p.bin”
