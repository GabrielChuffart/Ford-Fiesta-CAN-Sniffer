#include <Arduino.h>
#include "driver/twai.h"

// Définition des broches de l'ESP32 reliées au Transceiver
#define TX_PIN GPIO_NUM_5
#define RX_PIN GPIO_NUM_4

void setup() {
  Serial.begin(115200); // Vitesse du moniteur série pour PC
  Serial.println("Démarrage du Nœud A (Émetteur)...");

  // 1. Configuration du module CAN (TWAI)
  twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
  twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS(); // Vitesse standard automobile
  twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL(); // On n'applique pas de filtre au départ

  // 2. Installation et démarrage du driver
  if (twai_driver_install(&g_config, &t_config, &f_config) == ESP_OK) {
    Serial.println("Driver CAN installé avec succès.");
  } else {
    Serial.println("Erreur : Impossible d'installer le driver.");
    return;
  }

  if (twai_start() == ESP_OK) {
    Serial.println("Module CAN démarré et prêt à émettre.");
  } else {
    Serial.println("Erreur : Impossible de démarrer le module CAN.");
    return;
  }
}

void loop() {
  // 3. Création de la trame CAN
  twai_message_t message;
  message.identifier = 0x123; // ID fausse pédale d'accélérateur
  message.extd = 0;           // 0 = Format Standard (ID sur 11 bits)
  message.rtr = 0;            // 0 = Trame de données (Data Frame)
  message.data_length_code = 8; // On envoie 8 octets de données au maximum (DLC)

  // Remplissage des données (Simulation d'un appui à 42% sur la pédale)
  message.data[0] = 42; 
  // Mise à 0 du reste des octets pour faire propre
  for (int i = 1; i < 8; i++) {
    message.data[i] = 0;
  }

  // 4. Envoi de la trame sur le réseau
  if (twai_transmit(&message, pdMS_TO_TICKS(1000)) == ESP_OK) {
    Serial.println("Trame envoyée : [ID 0x123] - Data[0] : 42");
  } else {
    Serial.println("Erreur : Impossible d'envoyer la trame.");
  }

  delay(1000); // On met en pause une seconde avant le prochain envoi
}