# projeto-ECU
https://wokwi.com/projects/428399225052770305


✅ Funcionalidades Já Implementadas na ECU (ESP32)

🔧 Leitura de Sensores

TPS (Throttle Position Sensor): Potenciômetro lido via ADC. Usado para interpolação do mapa TPS x RPM e lógica de corte de injeção.

MAP (Manifold Absolute Pressure): Sensor de pressão lido via ADC. Utilizado para interpolação do mapa MAP x RPM.

Temperatura do motor (sensor NTC): Lida via ADC. Usada em cold start e afterstart enrichment.

⚙️ Controle de Injeção

Mapa de Injeção TPS x RPM: Mapa 2D com interpolação bilinear. Ideal para controle baseado em borboleta (tipo Alfa-N).

Mapa de Injeção MAP x RPM: Mapa 2D com interpolação bilinear. Ideal para carga real (tipo Speed-Density).

Interpolação bilinear entre pontos do mapa: Garante transição suave entre os valores da malha.

🌡️ Enriquecimentos Especiais

Cold Start: Injeta mais combustível quando o motor está frio, baseado na leitura do sensor de temperatura.

Afterstart Enrichment: Enriquecimento temporário após o motor dar partida. Duração e intensidade controladas.

🛑 Cortes e Condições Especiais

Corte de giro (Rev Limiter): Impede injeção acima de um limite de RPM.

Corte de injeção com TPS fechado: Economia de combustível durante desaceleração (cut-off).

🔧 Novas Funcionalidades

Blend Factor (Estratégia de Mistura entre Mapas TPS x RPM e MAP x RPM):

Transição Dinâmica entre os Mapas: A partir de um valor de RPM, é possível transitar dinamicamente entre os mapas de injeção TPS x RPM e MAP x RPM, utilizando um fator de mistura.

Curva de Transição: A transição entre os mapas é controlada por pontos de RPM e valores de blend que definem a contribuição proporcional de cada mapa (TPS x RPM e MAP x RPM) no cálculo do tempo de injeção.

Fatores de Mistura Definidos: O fator de mistura varia conforme a RPM, permitindo otimizar a resposta de combustível para diferentes condições de operação do motor.

Crank Enrichment (Enriquecimento Durante a Partida):

Enriquecimento Durante a Partida: Durante o processo de partida do motor (cranking), é aplicada uma correção de enriquecimento para fornecer combustível adicional ao motor.

Controle de Temperatura: O enriquecimento durante a partida também pode ser ajustado com base na temperatura do motor (usando o sensor NTC).

Fator de Enriquecimento Dinâmico: A intensidade do enriquecimento é ajustada com base em um fator dinâmico calculado durante o processo de cranking, otimizando a mistura de combustível para uma partida mais suave e eficiente.

🧪 Ambiente de Desenvolvimento

Simulação no Wokwi: Projeto testado em ambiente virtual, facilitando testes de lógica e sensores.

Plataforma ESP32 com Arduino/C++: Código customizado com controle direto de ADC e lógica de injeção.

Código customizado com controle direto de ADC e lógica de injeção.


