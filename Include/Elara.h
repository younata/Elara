#ifndef __ELARA_H__
#define __ELARA_H__

typedef void (^ElaraTestBlock)(void);

int elara_main(int argc, char *argv[]);

void beforeEach(ElaraTestBlock before);
void afterEach(ElaraTestBlock after);
void describe(const char *name, ElaraTestBlock context);
void it(const char *name, ElaraTestBlock test);

void elara_expect(int condition, const char *expression, const char *file, int line_number);
#define expect(expr)    elara_expect((expr), #expr, __FILE__, __LINE__)

#endif /* __ELARA_H__ */
