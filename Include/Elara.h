#ifndef __ELARA_H__
#define __ELARA_H__

typedef void (^ElaraTestBlock)(void);

int elara_main(int argc, char *argv[]);

void beforeEach(ElaraTestBlock before);
void afterEach(ElaraTestBlock after);
void describe(const char *name, ElaraTestBlock context);
void it(const char *name, ElaraTestBlock test);

void expect(int condition);

#endif /* __ELARA_H__ */
