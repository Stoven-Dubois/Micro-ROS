#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include "fonctions.h"
#include "structure.h"

//#ifndef MASTER_H
//#define MASTER_H

//#endif // LOGGER_H

#define MAXCOMMANDSIZE 1000
