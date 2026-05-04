#include <Arduino.h>
#include "driver/twai.h"

#define TX_PIN GPIO_NUM_5
#define RX_PIN GPIO_NUM_4

void setup() {
  Serial.begin(115200);
  Serial.println("Démarrage du Nœud B (Sniffer)...");

  // Configuration identique à l'émetteur
  // Note : Plus tard sur la voiture, passer en TWAI_MODE_LISTEN_ONLY pour la sécurité
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); 
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

  twai_driver_install(&g_config, &t_config, &f_config);
  twai_start();
  
  Serial.println("En écoute sur le bus CAN...");
}

void loop() {
  twai_message_t message;

  // 1. Le programme attend qu'une trame arrive
  if (twai_receive(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    
    // Trame reçue
    Serial.print("Trame reçue -> ID: 0x");
    Serial.print(message.identifier, HEX); // Affiche l'ID en Hexadécimal
    Serial.print(" | DLC: ");
    Serial.print(message.data_length_code);
    Serial.print(" | Données: ");
    
    // Boucle pour afficher le contenu de chaque octet
    for (int i = 0; i < message.data_length_code; i++) {
      Serial.print(message.data[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}