#include "led_app.h"
#include "math.h"

uint8_t ucLed[6] = {1,0,1,0,1,0};  // LED ״̬���� (6��LED)
/**
 * @brief ����ucLed����״̬����6��LED����ʾ
 * @param ucLed Led���ݴ������� (��СΪ6)
 */
void led_disp(uint8_t *ucLed)
{
    uint8_t temp = 0x00;                // ���ڼ�¼��ǰ LED ״̬����ʱ���� (���6λ��Ч)
    static uint8_t temp_old = 0xff;     // ��¼֮ǰ LED ״̬�ı���, �����ж��Ƿ���Ҫ������ʾ

    for (int i = 0; i < 6; i++)         // ����6��LED��״̬
    {
        // ��LED״̬���ϵ�temp�����У���������Ƚ�
        if (ucLed[i]) temp |= (1 << i); // ���ucLed[i]Ϊ1, ��temp�ĵ�iλ��1
    }

    // ������ǰ״̬��֮ǰ״̬��ͬ��ʱ�򣬲Ÿ�����ʾ
    if (temp != temp_old)
    {
        // ʹ��HAL�⺯������temp��ֵ���ö�Ӧ����״̬ (����ߵ�ƽ����)
        HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, (temp & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 1 (PD8)
        HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, (temp & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 2 (PD9)
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, (temp & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 3 (PD10)
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, (temp & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 4 (PD11)
        HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, (temp & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 5 (PD12)
        HAL_GPIO_WritePin(LED6_GPIO_Port, LED6_Pin, (temp & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 6 (PD13)

        temp_old = temp;                // ���¼�¼�ľ�״̬
    }
}

/**
 * @brief LED ��ʾ������ (��ѭ������)
 */
void led_task(void)
{
//	// ������ˮ����ر���
//    static uint32_t breathCounter = 0;      // ȫ�ֺ�����ʱ��
//    static uint8_t pwmCounter = 0;          // ���PWM������ (����LED����)
//    static const uint16_t breathPeriod = 3000; // ������ˮ����Ч�������� (����һ�㣬�������)
//    static const uint8_t pwmMax = 20;       // PWM���� (ͬ��)
//    // �ؼ�����λ�����������LED��������Ĵ��̶�
//    // �� / 3 ��ζ��һ����������(2��)�������� 6 ��LED (2�� / (��/3) = 6)
//    // ÿ��LED��ǰһ���� ��/3 ����λ��ʼ����
//    static const float phaseShift = 3.14159f / 3.0f;

//    // ����ȫ�ֺ�����ʱ��
//    breathCounter = (breathCounter + 1) % breathPeriod;

//    // ����PWM������
//    pwmCounter = (pwmCounter + 1) % pwmMax;

//    // ѭ��Ϊÿ��LED������������Ȳ�����״̬
//    for(uint8_t i = 0; i < 6; i++) // ��������6��LED
//    {
//        // ���㵱ǰLED����λ�� (angle)
//        // (2.0f * 3.14159f * breathCounter) / breathPeriod �ǻ����Ƕȣ���ʱ��仯
//        // - i * phaseShift ��Ϊ�� i ��LED�������λ�ӳ�
//        float angle = (2.0f * 3.14159f * breathCounter) / breathPeriod - i * phaseShift;

//        // ����ԭʼ������ֵ (-1.0 �� 1.0)
//        float sinValue = sinf(angle);

//        // ��ǿ�ԱȶȲ��������� (�������������ȫ��ʱ�����)
//        // powf(x, 0.5f) �൱�� sqrt(x)�����������������С�������ߣ���������ѹ��һ�� (��������Ҫ���� >0 �Ĳ���)
//        // ���ڸ���������ȡ����ֵ�ٿ����ţ�Ȼ��ӻظ��ţ�������״�Գ�
//        // ��ʹ�����ȴӰ������Ĺ��̱�죬���������Ĺ���Ҳ��죬�м�����ʱ������
//        float enhancedValue = sinValue > 0 ? powf(sinValue, 0.5f) : -powf(-sinValue, 0.5f);

//        // ��һ��ѹ���������ߣ�ʹ��ֻ���ڽӽ���ֵʱ�������ﵽ������
//        // �����ǿ���ֵ����0.7 (�ӽ���ֵ)���򱣳ֲ��䣻���򣬳���0.6����������
//        // Ŀ�����á��Ⲩ���Եø�խ�������и�ǿ
//        enhancedValue = enhancedValue > 0.7f ? enhancedValue : enhancedValue * 0.6f;

//        // �������� enhancedValue (-1 �� 1 ֮��) ӳ�䵽 0 �� pwmMax ������ֵ
//        uint8_t brightness = (uint8_t)((enhancedValue + 1.0f) * pwmMax / 2.0f);

//        // ���ݼ�����ĸ�LED�����ȣ�ʹ��PWM�߼�������״̬
//        ucLed[i] = (pwmCounter < brightness) ? 1 : 0;
//    }
    led_disp(ucLed); // ע�⣺led_disp�ڲ����Ҳ���Ż�������״̬����ʱ�ظ�дGPIO
//    led_disp(ucLed);                    // ����led_disp��������LED״̬
}
            