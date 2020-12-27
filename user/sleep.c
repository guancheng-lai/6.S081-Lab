#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#include <stddef.h>

int 
main(int argc, char* argv[]) {
    sleep(atoi(argv[1]));
    exit(0);
}
