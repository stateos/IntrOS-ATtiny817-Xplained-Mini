#include "os.h"
#include "led.h"

using namespace intros;

auto sem = Semaphore::Binary(0);

auto sla = Task::Start([]
{
	sem.wait();
	led_toggle(0);
});

auto mas = Task::Start([]
{
	This::sleepFor(SEC);
	sem.give();
});

int main()
{
	led_init(0);
	This::exit();
}
