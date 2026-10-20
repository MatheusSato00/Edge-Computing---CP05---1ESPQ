# 🍷 Vinheria — Monitoramento Inteligente de Armazenamento de Vinhos

Sistema de Internet das Coisas (IoT) desenvolvido para monitorar as condições ambientais de um depósito de vinhos, utilizando sensores conectados a um ESP32 e integração com a plataforma FIWARE.

O projeto tem como objetivo acompanhar a temperatura, a umidade relativa do ar e a luminosidade do ambiente, permitindo centralizar as informações coletadas e preparar a infraestrutura para análises históricas das condições de armazenamento.

> **Status do projeto:** 🚧 Em desenvolvimento — back-end FIWARE em funcionamento e integração física dos sensores em andamento.

## 📌 Sobre o Projeto

A conservação adequada dos vinhos depende de fatores ambientais, como temperatura, umidade e exposição à luz. O monitoramento dessas condições permite identificar variações no ambiente e obter informações que auxiliem na avaliação das condições de armazenamento.

Para atender a essa necessidade, o projeto Vinheria utiliza um ESP32 conectado a um sensor DHT11 e a um sensor LDR. A proposta é coletar os dados ambientais, transmiti-los por Wi-Fi utilizando MQTT e disponibilizá-los no FIWARE por meio do Orion Context Broker e da interface NGSIv2.

A arquitetura também prevê o armazenamento histórico das leituras, possibilitando consultas e análises posteriores quando o componente de persistência estiver configurado e integrado.

## 🎯 Objetivos

- Monitorar a luminosidade do ambiente de armazenamento.
- Medir a temperatura e a umidade relativa do ar.
- Coletar dados ambientais utilizando um ESP32.
- Transmitir as informações por Wi-Fi e MQTT.
- Integrar as leituras à infraestrutura FIWARE.
- Disponibilizar os dados de contexto por meio da interface NGSIv2.
- Associar data e hora às leituras para facilitar a análise temporal.
- Preparar a solução para persistência, consulta e exportação de dados históricos.
- Aplicar conceitos de IoT, comunicação entre dispositivos e interoperabilidade.

## 🌡️ Importância do Monitoramento

O acompanhamento das condições ambientais pode contribuir para a identificação de situações que merecem atenção durante o armazenamento dos vinhos.

| Fator monitorado | Referência aproximada | Importância |
|---|---|---|
| Temperatura | 13 °C | Ajuda a identificar variações térmicas que podem afetar a conservação. |
| Umidade relativa | 70% | Auxilia no acompanhamento das condições de umidade do ambiente. |
| Luminosidade | Baixa exposição à luz | Permite identificar variações na iluminação do depósito. |

*Os valores apresentados são referências gerais para o projeto, não limites universais. As condições ideais dependem do tipo de vinho, da duração do armazenamento e das características do ambiente.*

## 🛠️ Tecnologias e Componentes

### Hardware

| Componente | Função |
|---|---|
| ESP32 | Realizar a leitura dos sensores e a comunicação Wi-Fi. |
| DHT11 | Medir a temperatura e a umidade relativa do ar. |
| LDR | Detectar variações na intensidade luminosa. |
| Placa de montagem e conexões | Interligar os componentes eletrônicos. |

### Software e protocolos

- **C/C++ para Arduino:** desenvolvimento do firmware, conforme o ambiente utilizado.
- **ESP32:** plataforma de processamento embarcado.
- **Wi-Fi:** comunicação do dispositivo com a rede.
- **MQTT:** protocolo de troca de mensagens entre dispositivos e serviços.
- **FIWARE Orion Context Broker:** gerenciamento dos dados de contexto.
- **NGSIv2:** interface para consulta e atualização das entidades de contexto.
- **MongoDB e serviços complementares:** componentes do ambiente de infraestrutura, conforme a configuração efetivamente utilizada.

A utilização de Smart Data Models pode ser considerada na definição de estruturas de dados interoperáveis, desde que o modelo escolhido seja implementado e validado no projeto.

## 🏗️ Arquitetura da Solução

O fluxo de comunicação previsto para o sistema é:

```text
┌───────────────────────────┐
│     Sensores ambientais   │
│       DHT11 + LDR         │
└─────────────┬─────────────┘
              │
              ▼
┌───────────────────────────┐
│           ESP32           │
│  Leitura e processamento  │
│       dos sensores        │
└─────────────┬─────────────┘
              │
              │ Wi-Fi / MQTT
              ▼
┌───────────────────────────┐
│    Broker MQTT e camada   │
│        de integração      │
└─────────────┬─────────────┘
              │
              ▼
┌───────────────────────────┐
│   Orion Context Broker    │
│         NGSIv2            │
└─────────────┬─────────────┘
              │
              ▼
┌───────────────────────────┐
│ Persistência e consultas  │
│ históricas (se habilitadas)│
└───────────────────────────┘
```

### Fluxo de funcionamento

1. Os sensores realizam as medições ambientais.
2. O ESP32 lê os valores e prepara as informações para transmissão.
3. O dispositivo conecta-se à rede Wi-Fi.
4. As mensagens são publicadas por MQTT.
5. A camada de integração encaminha os dados ao Orion Context Broker.
6. O Orion disponibiliza os dados de contexto por meio da interface NGSIv2.
7. Um componente de persistência, quando configurado, registra as alterações para consultas históricas.

**Observação técnica:** o Orion Context Broker, o broker MQTT e o mecanismo de persistência possuem funções distintas. A presença de um componente no ambiente não garante que todas essas integrações estejam configuradas ou funcionando.

## 📊 Dados Monitorados

Os atributos previstos para representar cada leitura são:

| Atributo | Descrição | Unidade ou formato |
|---|---|---|
| `temperature` | Temperatura ambiente | °C |
| `relativeHumidity` | Umidade relativa do ar | % |
| `luminosity` | Valor obtido ou calculado a partir do LDR | Escala definida pela implementação |
| `timestamp` | Data e hora da coleta | Data e hora em formato documentado |

Os nomes definitivos, os tipos dos atributos e o formato temporal devem corresponder à implementação do firmware e ao modelo de entidade utilizado no FIWARE.

**Importante:** o LDR não fornece diretamente uma medição calibrada em lux. Para apresentar a luminosidade nessa unidade, é necessário definir o circuito de leitura e realizar uma calibração adequada.

## ⚙️ Requisitos para Execução

### Hardware

- Placa ESP32.
- Sensor DHT11.
- Sensor LDR.
- Componentes para montagem e conexões elétricas.
- Cabo USB compatível com a placa.

### Software e infraestrutura

- Computador com ambiente de desenvolvimento compatível com ESP32.
- Bibliotecas utilizadas pelo firmware.
- Rede Wi-Fi disponível.
- Back-end FIWARE configurado.
- Broker MQTT e mecanismo de integração compatíveis com o firmware.
- Componente de persistência, caso sejam necessárias consultas históricas.

As versões das ferramentas, das bibliotecas e dos serviços devem ser documentadas conforme o ambiente utilizado na versão final do projeto.

## 🚀 Como Executar

Os passos abaixo descrevem o procedimento geral. Os comandos, endereços, tópicos MQTT e parâmetros definitivos devem ser obtidos dos arquivos de configuração e do firmware do projeto.

### 1. Preparar o back-end

Inicie os serviços FIWARE utilizando o arquivo de configuração da implantação.

Caso o projeto utilize Docker Compose, verifique o estado dos contêineres com:

```bash
sudo docker compose ps
```

Se necessário, informe o caminho do arquivo de configuração utilizado no ambiente.

Confirme a disponibilidade do Orion Context Broker, do broker MQTT e dos serviços de integração necessários.

### 2. Configurar o dispositivo

Abra o firmware do ESP32 no ambiente de desenvolvimento utilizado pelo projeto.

Configure os parâmetros necessários, incluindo:

- Nome da rede Wi-Fi.
- Senha da rede Wi-Fi.
- Endereço e porta do broker MQTT.
- Tópico utilizado para publicação.
- Parâmetros exigidos pela integração com o FIWARE.

Utilize valores compatíveis com a infraestrutura real. Não publique senhas, tokens ou outras credenciais no repositório.

### 3. Montar o circuito

Realize as conexões do ESP32, do DHT11 e do LDR de acordo com o esquema elétrico definitivo.

Antes de ligar o circuito, confira a alimentação, a identificação dos pinos e a compatibilidade elétrica dos componentes com o ESP32.

Os pinos utilizados devem ser documentados no repositório após a confirmação da montagem física.

### 4. Compilar e gravar o firmware

Compile o código e grave-o na placa ESP32 utilizando a ferramenta compatível com o projeto.

Após a gravação, abra o monitor serial e verifique as mensagens de inicialização, a leitura dos sensores e o estado das conexões.

### 5. Validar as leituras

Confirme se o dispositivo consegue obter valores de temperatura, umidade relativa e luminosidade.

Verifique se os valores são coerentes com o ambiente e se as unidades e escalas estão documentadas.

### 6. Validar a comunicação MQTT

Verifique se o ESP32 conecta-se ao broker e publica mensagens no tópico configurado.

Confira o conteúdo das mensagens, os nomes dos atributos e a associação da data e hora às leituras, conforme o firmware implementado.

### 7. Consultar os dados no FIWARE

Utilize a interface NGSIv2 do Orion Context Broker para consultar a entidade correspondente ao dispositivo.

Exemplo de consulta genérica:

```http
GET /v2/entities
```

O endereço completo, os cabeçalhos e os parâmetros de consulta devem ser ajustados à configuração real do ambiente.

Verifique se os atributos recebidos correspondem às informações publicadas pelo dispositivo.

### 8. Verificar o histórico

Caso a persistência esteja habilitada, consulte o serviço responsável pelo armazenamento histórico.

Confirme se as leituras anteriores podem ser recuperadas e se os dados podem ser exportados no formato disponibilizado pela implementação.

## 🧪 Testes de Validação

A validação deve contemplar as seguintes etapas:

- [ ] Inicialização do ESP32.
- [ ] Leitura do DHT11.
- [ ] Leitura do LDR.
- [ ] Conexão à rede Wi-Fi.
- [ ] Conexão ao broker MQTT.
- [ ] Publicação de mensagens MQTT.
- [ ] Recebimento dos dados pela camada de integração.
- [ ] Atualização dos atributos no Orion Context Broker.
- [ ] Consulta das entidades pela NGSIv2.
- [ ] Persistência e recuperação do histórico, caso configuradas.
- [ ] Exportação dos dados, caso implementada.

Marque cada etapa somente após executar o teste e confirmar o resultado. A validação ponta a ponta deve demonstrar o caminho desde a leitura dos sensores até a disponibilização das informações no FIWARE.

## 📈 Resultados Esperados

Com a integração completa, espera-se que a solução permita:

- Acompanhar as condições ambientais do depósito de vinhos.
- Consultar informações de temperatura, umidade e luminosidade.
- Transmitir dados ambientais por Wi-Fi e MQTT.
- Disponibilizar informações de contexto por meio do FIWARE.
- Analisar variações ambientais ao longo do tempo, quando houver persistência histórica.
- Preparar os dados para a elaboração de gráficos e relatórios.
- Facilitar futuras integrações com sistemas externos compatíveis.

Os resultados efetivamente obtidos deverão ser registrados após a execução dos testes de validação.

## ⚠️ Limitações e Estado Atual

O back-end FIWARE está em funcionamento, enquanto a montagem física dos sensores e a integração completa com o dispositivo ainda estão em desenvolvimento.

As principais limitações consideradas nesta etapa são:

- O DHT11 possui precisão e faixa de operação limitadas.
- O LDR exige calibração para que suas leituras sejam interpretadas como valores de luminosidade em unidades físicas.
- A comunicação entre o ESP32 e o FIWARE depende da configuração correta do broker MQTT e da camada de integração.
- A consulta histórica depende da configuração e do funcionamento do componente de persistência.
- A integração completa ainda precisa ser validada por testes ponta a ponta.

## 🔮 Próximas Etapas

- [ ] Finalizar a montagem física dos sensores.
- [ ] Validar as leituras no monitor serial.
- [ ] Estabelecer a comunicação entre o ESP32 e o broker MQTT.
- [ ] Confirmar a atualização dos dados no Orion Context Broker.
- [ ] Documentar os pinos e as conexões elétricas.
- [ ] Registrar as versões das ferramentas e das bibliotecas.
- [ ] Validar a persistência e as consultas históricas, caso previstas na implantação.
- [ ] Documentar os resultados dos testes e incluir evidências da execução.

## 👥 Equipe

| Integrante | Registro acadêmico |
|---|---|
| Matheus Sato Oliveira do Prado | RM 569392 |
| Matheus Mendes Duarte da Silva | RM 569559 |

---

**Projeto acadêmico de IoT — Vinheria**

Desenvolvido com foco em monitoramento ambiental, sistemas embarcados, comunicação MQTT e integração com a plataforma FIWARE.
 
