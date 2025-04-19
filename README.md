# projeto-ECU
https://wokwi.com/projects/428399225052770305


✅ Funcionalidades Já Implementadas na ECU (ESP32)

🔧 Leitura de Sensores
TPS (Throttle Position Sensor)

Potenciômetro lido via ADC.

Usado para interpolação do mapa TPS x RPM e lógica de corte de injeção.

MAP (Manifold Absolute Pressure)

Sensor de pressão lido via ADC.

Utilizado para interpolação do mapa MAP x RPM.

Temperatura do motor (sensor NTC)

Lida via ADC.

Usada em cold start e afterstart enrichment.

⚙️ Controle de Injeção
Mapa de Injeção TPS x RPM

Mapa 2D com interpolação bilinear.

Ideal para controle baseado em borboleta (tipo Alfa-N).

Mapa de Injeção MAP x RPM

Mapa 2D com interpolação bilinear.

Ideal para carga real (tipo Speed-Density).

Interpolação bilinear entre pontos do mapa

Garante transição suave entre os valores da malha.

🌡️ Enriquecimentos Especiais
Cold Start

Injeta mais combustível quando o motor está frio.

Baseado na leitura do sensor de temperatura.

Afterstart Enrichment

Enriquecimento temporário após o motor dar partida.

Duração e intensidade controladas.

🛑 Cortes e Condições Especiais
Corte de giro (Rev Limiter)

Impede injeção acima de um limite de RPM.

Corte de injeção com TPS fechado

Economia de combustível durante desaceleração (cut-off).

🧪 Ambiente de Desenvolvimento
Simulação no Wokwi

Projeto testado em ambiente virtual.

Facilita testes de lógica e sensores.

Plataforma ESP32 com Arduino/C++

Código customizado com controle direto de ADC e lógica de injeção.


