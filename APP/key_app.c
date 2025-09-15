#include "key_app.h"

/*=====================================����������ѡ��====================================*/
#define KEY_SCHEME_SIMPLE    1  // �򵥰�������
#define KEY_SCHEME_ADVANCED  2  // �߼���������(ebtn��)

// ��ǰʹ�õķ��� - �����������л���ͬ�İ���������
#define CURRENT_KEY_SCHEME   KEY_SCHEME_ADVANCED

/*=====================================�򵥰���������====================================*/

#if (CURRENT_KEY_SCHEME == KEY_SCHEME_SIMPLE)

uint8_t Key_Val,Key_Up,Key_Down,Key_Old;

uint8_t Key_Read()
{
	uint8_t temp = 0;
	if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)==GPIO_PIN_RESET) temp = 1; //KEY1
    if(HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)==GPIO_PIN_RESET) temp = 2; //KEY2
    if(HAL_GPIO_ReadPin(KEY3_GPIO_Port,KEY3_Pin)==GPIO_PIN_RESET) temp = 3; //KEY3
    if(HAL_GPIO_ReadPin(KEY4_GPIO_Port,KEY4_Pin)==GPIO_PIN_RESET) temp = 4; //KEY4
    if(HAL_GPIO_ReadPin(KEY5_GPIO_Port,KEY5_Pin)==GPIO_PIN_RESET) temp = 5; //KEY5
    if(HAL_GPIO_ReadPin(KEY6_GPIO_Port,KEY6_Pin)==GPIO_PIN_RESET) temp = 6; //KEY6
	return temp;
}

void key_task()
{
    Key_Val = Key_Read(); // ��ȡ��ǰ����ֵ
    Key_Down = Key_Val & (Key_Old ^ Key_Val);   // ��������
    Key_Up = ~Key_Val &(Key_Old ^ Key_Val);     // �����ɿ�
    Key_Old = Key_Val;  // �����ϴΰ���ֵ
}


#endif /* CURRENT_KEY_SCHEME == KEY_SCHEME_SIMPLE */
/*=====================================�߼�����������(ebtn��)====================================*/

#if (CURRENT_KEY_SCHEME == KEY_SCHEME_ADVANCED)

#include "ebtn.h"

/* ������������ */
const ebtn_btn_param_t key_param_normal = EBTN_PARAMS_INIT(
    20,     // time_debounce: ��������ʱ�� 20ms
    20,     // time_debounce_release: �ͷ�����ʱ�� 20ms
    50,     // time_click_pressed_min: ��Ч�����С����ʱ�� 50ms
    500,    // time_click_pressed_max: ��Ч��������ʱ�� 500ms
    300,    // time_click_multi_max: ������ʱ�� 300ms
    500,    // time_keepalive_period: ���������¼����� 500ms
    5       // max_consecutive: ����������� 5 ��
);

/* ����ID���� */
typedef enum
{
	KEY1 = 1, // ��������ID
    KEY2,
    KEY3,
    KEY4,
    KEY5,
    KEY6,
    KEY_MAX,
	
	
/*��ϰ�������*/	
    COM_KEY1 = 101, // ��ϼ�ID
    COM_KEY2 = 102,
		COM_KEY3 = 103,
}user_button_t;
/* ������������ */
ebtn_btn_t my_buttons[] = {
    EBTN_BUTTON_INIT(KEY1, &key_param_normal),
    EBTN_BUTTON_INIT(KEY2, &key_param_normal),
    EBTN_BUTTON_INIT(KEY3, &key_param_normal),
    EBTN_BUTTON_INIT(KEY4, &key_param_normal),
    EBTN_BUTTON_INIT(KEY5, &key_param_normal),
    EBTN_BUTTON_INIT(KEY6, &key_param_normal),
};

/* ��ϰ������� */
ebtn_btn_combo_t my_combos[] = {
    EBTN_BUTTON_COMBO_INIT(COM_KEY1, &key_param_normal),
    EBTN_BUTTON_COMBO_INIT(COM_KEY2, &key_param_normal),
		EBTN_BUTTON_COMBO_INIT(COM_KEY3, &key_param_normal)
};

/* ����״̬��ȡ���� */
uint8_t my_get_key_state(struct ebtn_btn *btn) {
    switch (btn->key_id) {
        case KEY1:
            return (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET);
        case KEY2:
            return (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET);
        case KEY3:
            return (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET);
        case KEY4:
            return (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == GPIO_PIN_RESET);
        case KEY5:
            return (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == GPIO_PIN_RESET);
        case KEY6:
            return (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == GPIO_PIN_RESET);
        default:
            return 0;
    }
}
/* ��ϼ�״̬���� */
static uint8_t combo_key_active = 0;  // ��ϼ������־
static uint32_t combo_clear_time = 0; // ��ϼ����ʱ��
static uint8_t combo_operation_done = 0; // ��ϼ�������ɱ�־

/* �����¼������� */
void my_handle_key_event(struct ebtn_btn *btn, ebtn_evt_t evt) {
    uint16_t key_id = btn->key_id;
    uint16_t click_cnt = ebtn_click_get_count(btn);
    uint32_t current_time = HAL_GetTick();

    // �����ϼ�״̬�Ƿ���Ҫ���
    if (combo_key_active && (current_time - combo_clear_time > 300)) {
        combo_key_active = 0;
        combo_operation_done = 0;
    }

    switch (evt) {
        case EBTN_EVT_ONPRESS: // ��������
            // ������ϰ��� - ֻ��ONPRESS��ִ��ʵ�ʲ����������ظ�
            if (key_id >= COM_KEY1 && key_id <= COM_KEY3) {
                combo_key_active = 1;
                combo_clear_time = current_time;
                
                // ִֻ��һ�β���
                if (!combo_operation_done) {
                    combo_operation_done = 1;
                    
                    if (key_id == COM_KEY1) 
                    { 
                        // Copy_Key (KEY1+KEY2) ���ƹ���

                    }
                    else if(key_id == COM_KEY2)
                    {
                        //Paste_Key (KEY1 + KEY3��ճ������

                    }
                    else if(key_id == COM_KEY3)
                    {
                        //Cut_Key (KEY1+KEY4) ���й���

                       
                    }
                }
            }            
            // �������������ϸ��飺����ϼ��������޲�����ɱ�־��
            else if (!combo_key_active && !combo_operation_done) {
                if (key_id == KEY1) { /* KEY1 ���´��� */ }
                else if (key_id == KEY2) { /* KEY2 ���´��� */ }
                else if (key_id == KEY3) { /* KEY3 ���´��� */ }
                else if (key_id == KEY4) { /* KEY4 ���´��� */ }
                else if (key_id == KEY5) { /* KEY5 ���´��� */ }
                else if (key_id == KEY6) { /* KEY6 ���´��� */ }
            }
            break;

        case EBTN_EVT_ONRELEASE: // �����ͷ�
            // ������ϰ���
            if (key_id >= COM_KEY1 && key_id <= COM_KEY3) {
                // ��ϼ��ͷŴ���
                if (key_id == COM_KEY1) { 
                    // COM_KEY1 (KEY1+KEY2) �ͷ�
                }
                // ������ϼ����ʱ�䣬�ӳ�������
                combo_clear_time = current_time;
            }
            // �������������ϸ��飺����ϼ��������޲�����ɱ�־��
            else if (!combo_key_active && !combo_operation_done) {
                if (key_id == KEY1) { /* KEY1 �ͷŴ��� */ }
                else if (key_id == KEY2) { /* KEY2 �ͷŴ��� */ }
                else if (key_id == KEY3) { /* KEY3 �ͷŴ��� */ }
                else if (key_id == KEY4) { /* KEY4 �ͷŴ��� */ }
                else if (key_id == KEY5) { /* KEY5 �ͷŴ��� */ }
                else if (key_id == KEY6) { /* KEY6 �ͷŴ��� */ }
            }
            break;
        case EBTN_EVT_ONCLICK: // �������
            // ������ϰ��� 
           if (key_id >= COM_KEY1 && key_id <= COM_KEY3) {
                combo_key_active = 1;
                combo_clear_time = current_time;
            }
            // �������������ϸ��飺����ϼ��������޲�����ɱ�־��
            else if (!combo_key_active && !combo_operation_done) {
                if (key_id == KEY1) {
                    if (click_cnt == 1) {
                        // KEY1 ��������
                    } 
										else if (click_cnt == 2) 
										{
                        // KEY1 ˫������
                    }
                } 
                else if (key_id == KEY2) {
                    if (click_cnt == 1) {
                        // KEY2 ��������
                    }
                }
                else if (key_id == KEY3) {
                    if (click_cnt == 1) {
                        // KEY3 ��������
                    }
                }
                else if (key_id == KEY4) {
                    if (click_cnt == 1) {
                        // KEY4 ��������
                    }
                }
                else if (key_id == KEY5) {
                    if (click_cnt == 1) {
                        // KEY5 ��������
                    }
                }
                else if (key_id == KEY6) {
                    if (click_cnt == 1) {
                        // KEY6 ��������
                    }
                }
            }
            break;
        case EBTN_EVT_KEEPALIVE: // ��������
            // ������ϰ���
           if (key_id >= COM_KEY1 && key_id <= COM_KEY3) {
                combo_key_active = 1;
                combo_clear_time = current_time;
                
                if (key_id == COM_KEY1) {
                    // COM_KEY1 (KEY1+KEY2) ��������
                }
                else if (key_id == COM_KEY3) {
                    // Cut_Key (KEY1+KEY4) ��������
                }
            }
            // �������������ϸ��飺����ϼ��������޲�����ɱ�־��
            else if (!combo_key_active && !combo_operation_done) {
                if (key_id == KEY1) {
                    // KEY1 ��������
                }
                else if (key_id == KEY2) {
                    // KEY2 ��������
                }
                else if (key_id == KEY3) {
                    // KEY3 ��������
                }
                else if (key_id == KEY4) {
                    // KEY4 ��������
                }
                else if (key_id == KEY5) {
                    // KEY5 ��������
                }
                else if (key_id == KEY6) {
                    // KEY6 ��������
                }
            }
            break;

        default:
            break;
    }
}
/* ebtn��ʼ�� */
void my_ebtn_init(void)
{
    // ��ʼ��
    ebtn_init(my_buttons,EBTN_ARRAY_SIZE(my_buttons),my_combos,EBTN_ARRAY_SIZE(my_combos),my_get_key_state,my_handle_key_event);
    
    // ������ϼ�
    int key1_index = ebtn_get_btn_index_by_key_id(KEY1);
    int key2_index = ebtn_get_btn_index_by_key_id(KEY2);
	int key3_index = ebtn_get_btn_index_by_key_id(KEY3);
	int key4_index = ebtn_get_btn_index_by_key_id(KEY4);

    if (key1_index >= 0 && key2_index >= 0)
	{
        ebtn_combo_btn_add_btn_by_idx(&my_combos[0], key1_index);
        ebtn_combo_btn_add_btn_by_idx(&my_combos[0], key2_index);
	}
	if(key1_index >= 0 && key3_index >= 0)
	{
		ebtn_combo_btn_add_btn_by_idx(&my_combos[1], key1_index);
        ebtn_combo_btn_add_btn_by_idx(&my_combos[1], key3_index);
	}
	if(key1_index >= 0 && key4_index >= 0)
	{
		ebtn_combo_btn_add_btn_by_idx(&my_combos[2], key1_index);
        ebtn_combo_btn_add_btn_by_idx(&my_combos[2], key4_index);			
	}
}



void key_task() 
{
	ebtn_process(HAL_GetTick());
}

#endif /* CURRENT_KEY_SCHEME == KEY_SCHEME_ADVANCED */