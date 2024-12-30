#include "stm32f4xx_hal.h"
#include <string.h>

// UART handle
UART_HandleTypeDef huart2;

// Buffer untuk data yang diterima
uint8_t rx_buffer[100];
uint8_t rx_data;
uint8_t rx_index = 0;

// Inisialisasi UART
void MX_USART2_UART_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) {
        // Initialization Error
        Error_Handler();
    }
}

// Callback untuk menerima data
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rx_data != '\n') {
            rx_buffer[rx_index++] = rx_data;
        } else {
            rx_buffer[rx_index] = '\0';
            rx_index = 0;
            // Proses data yang diterima
            processData(rx_buffer);
        }
        HAL_UART_Receive_IT(&huart2, &rx_data, 1);
    }
}

// Fungsi untuk memproses data yang diterima
void processData(uint8_t *data) {
    // Tampilkan data ke terminal
    HAL_UART_Transmit(&huart2, data, strlen((char *)data), HAL_MAX_DELAY);
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    // Mulai menerima data
    HAL_UART_Receive_IT(&huart2, &rx_data, 1);

    while (1) {
        // Loop utama
    }
}

// Fungsi konfigurasi clock sistem
void SystemClock_Config(void) {
    // Konfigurasikan clock sesuai dengan kebutuhan
}

// Fungsi inisialisasi GPIO
void MX_GPIO_Init(void) {
    // Inisialisasi pin GPIO sesuai kebutuhan
}

// Fungsi error handler
void Error_Handler(void) {
    // Tangani error
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
    // Tangani assert error
}
#endif

void sendATCommand(char *command) {
    HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), HAL_MAX_DELAY);
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USART2_UART_Init();

    // Mulai menerima data
    HAL_UART_Receive_IT(&huart2, &rx_data, 1);

    // Inisialisasi ESP8266
    sendATCommand("AT\r\n");
    HAL_Delay(1000);
    sendATCommand("AT+CWMODE=1\r\n");
    HAL_Delay(1000);
    sendATCommand("AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n");
    HAL_Delay(5000);
    sendATCommand("AT+CIPSTART=\"TCP\",\"api.covid19api.com\",80\r\n");
    HAL_Delay(2000);
    sendATCommand("AT+CIPSEND=GET /summary HTTP/1.1\r\nHost: api.covid19api.com\r\n\r\n");
    HAL_Delay(2000);

    while (1) {
        // Loop utama
    }
}