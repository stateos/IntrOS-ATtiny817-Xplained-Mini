#ifndef __INIT_H__
#define __INIT_H__

#define __INIT(N) __attribute__((used, naked, optimize("s"), section(".init"#N)))
#define __FINI(N) __attribute__((used, naked, optimize("s"), section(".fini"#N)))

#endif//__INIT_H__
