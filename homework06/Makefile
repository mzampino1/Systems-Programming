CC=		gcc
CFLAGS=		-Wall -g -std=gnu99 -fPIC
LD=		gcc
LDFLAGS=	-L.
LIBS=		-lstr
AR=		ar
ARFLAGS=	rcs
TARGETS=	libstr.so libstr.a trit.dynamic trit.static

all:		$(TARGETS)

#-------------------------------------------------------------------------------
# Add rules for libstr.a libstr.so
#-------------------------------------------------------------------------------

str.o:	str.c	str.h
	$(CC) $(CFLAGS) -c -o str.o str.c

libstr.so:	str.o
	$(LD) $(LDFLAGS) -shared -o libstr.so str.o

libstr.a:	str.o
	$(AR) $(ARFLAGS) libstr.a str.o


#-------------------------------------------------------------------------------
# Add rules for trit.dynamic trit.static
#-------------------------------------------------------------------------------

trit.o:		trit.c	str.h
	$(CC) $(CFLAGS) -c -o trit.o trit.c

trit.dynamic:	libstr.so	trit.o
	$(LD) $(LDFLAGS) -o trit.dynamic trit.o $(LIBS)

trit.static:	libstr.a	trit.o
	$(LD) $(LDFLAGS) -static -o trit.static trit.o $(LIBS)

#-------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#-------------------------------------------------------------------------------

test:
	@$(MAKE) -sk test-all

test-all:	test-gitignore
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-gitignore:
	@echo "*.a" > .gitignore
	@echo "*.o" >> .gitignore
	@echo "*.py" >> .gitignore
	@echo "*.sh" >> .gitignore
	@echo "*.dynamic" >> .gitignore
	@echo "*.static" >> .gitignore
	@echo "*.so" >> .gitignore
	@echo "*.unit" >> .gitignore

test-libstr:	test-libstr.so test-libstr.a

test-libstr.so:	libstr.so
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework06/str.test.py
	@chmod +x str.test.py
	@./str.test.py -v

test-libstr.a:	str.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework06/str.unit.sh
	@chmod +x str.unit.sh
	@./str.unit.sh

str.unit:	str.unit.c libstr.a
	@$(CC) $(CFLAGS) -o str.unit str.unit.c libstr.a

test-trit:	trit.dynamic trit.static
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework06/trit.test.sh
	@chmod +x trit.test.sh
	@./trit.test.sh

clean:
	@rm -f str.o libstr.so libstr.a str.test.py str.unit str.unit.sh trit.o trit.dynamic trit.static trit.test.sh

#-------------------------------------------------------------------------------
# Embedded Grade Summary Python Script
#-------------------------------------------------------------------------------

define SUMMARIZE_GRADE_PY
import os

def run_and_extract(cmd):
    grade  = 0.0
    status = 'Failure'
    for line in os.popen(cmd):
        if 'Grade' in line:  grade  = line.split()[1]
        if 'Status' in line: status = line.split()[1]
        print(line, end='')
    return float(grade), status == 'Success'

strd_grade, strd_status = run_and_extract('make test-libstr.so')
strs_grade, strs_status = run_and_extract('make test-libstr.a')
trit_grade, trit_status = run_and_extract('make test-trit')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
all_success = all([strd_status, strs_status, trit_status, quiz_status])

print('Grade Summary ...')
print('  Str[d]  {:4.2f} /  0.25'.format(0.25*strd_grade))
print('  Str[s]  {:4.2f} /  0.25'.format(0.25*strs_grade))
print('  Trit    {:4.2f} /  0.30'.format(0.30*trit_grade))
print('  Quiz    {:4.2f} /  0.20'.format(0.20*quiz_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.25*strd_grade + 0.25*strs_grade + 0.30*trit_grade + 0.20*quiz_grade
))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
