#include <os.h>
#include <led.h>

OS_SEM(sem, 0);

OS_TSK_DEF(sla)
{
	sem_wait(sem);
	led_toggle();
}

OS_TSK_DEF(mas)
{
	tsk_delay(SEC);
	sem_give(sem);
}

int main()
{
	led_init();
	tsk_start(sla);
	tsk_start(mas);
	tsk_stop();
}
