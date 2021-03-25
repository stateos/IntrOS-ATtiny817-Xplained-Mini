#include "os.h"
#include "led.h"

using namespace intros;

auto sem = Semaphore::Binary(0);

auto sla = Task::Start([]
{
	sem.wait();
	led_toggle();
});

auto mas = Task::Start([]
{
	this_task::sleepFor(SEC);
	sem.give();
});

int main()
{
	led_init();
	this_task::exit();
}
