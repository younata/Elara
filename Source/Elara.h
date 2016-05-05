#ifndef __ELARA_H__
#define __ELARA_H__

typedef void (^ElaraTestBlock)(void);

void elara_tests(ElaraTestBlock tests);
void describe(const char *name, ElaraTestBlock context);
void it(const char *name, ElaraTestBlock test);

void expect(int condition);

#endif /* __ELARA_H__ */
