#include <os.h>
#include <led.h>

OS_SEM(sem, 0, semBinary);

OS_TSK_START(sla, 0)
{
	sem_wait(sem);
	led_toggle();
}

OS_TSK_START(mas, 0)
{
	tsk_delay(SEC);
	sem_give(sem);
}

int main()
{
	led_init();
	tsk_sleep();
}
