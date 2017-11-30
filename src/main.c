#include <os.h>
#include <led.h>

OS_SEM(sem, 0, semBinary);

OS_TSK_START(sla)
{
	sem_wait(sem);
	led_toggle();
}

OS_TSK_START(mas)
{
	tsk_delay(SEC);
	sem_give(sem);
}

int main()
{
	led_init();
	tsk_sleep();
}
