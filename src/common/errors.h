#ifndef _ERRORS_H_
#  define _ERRORS_H_

void init_errors(int level, FILE * fp);
void syntax(char* str, ...);
void scanner_error(char* str, ...);
void warning(char* str, ...);
void debug(int level, char* str, ...);
void fatal_error(char* str, ...);
void expect(char* exp, int tok);

void set_logging_level(int lev);
int get_logging_level(void);
int get_num_errors(void);
int get_num_warnings(void);
void set_error_level(int lev);
int get_error_level(void);
void set_error_stream(FILE * fp);
FILE* get_error_stream(void);
void inc_error_count(void);
void inc_warning_count(void);

void debug_mark(int, const char* , int, const char* );
void debug_msg(int, const char* , ...);

/*
 * TODO: Add more macros for debugging and memory tracking.
 */
#  ifdef TRACE
#    define MARK()      debug_mark(5, __FILE__, __LINE__, __func__)
#    define MSG(...)    debug_msg(5, __VA_ARGS__)
#  else
#    define MARK()
#    define MSG(...)
#  endif

#endif /* _ERRORS_H_ */
