#ifndef __ELARA_H__
#define __ELARA_H__

#include "ElaraTypes.h"
#include "Matchers.h"

typedef void (^ElaraTestBlock)(void);

int elara_main(int argc, char *argv[]);

void beforeEach(ElaraTestBlock before);
void afterEach(ElaraTestBlock after);
void describe(const char *name, ElaraTestBlock context);
void fdescribe(const char *name, ElaraTestBlock context);
void xdescribe(const char *name, ElaraTestBlock context);
void it(const char *name, ElaraTestBlock test);
void fit(const char *name, ElaraTestBlock test);
void xit(const char *name, ElaraTestBlock test);

#endif /* __ELARA_H__ */
