CC=		gcc
CFLAGS=		-Wall -std=gnu99 -g
LD=		gcc
LDFLAGS=	-L.
TARGETS=	findit

all:		$(TARGETS)

#-------------------------------------------------------------------------------
# Rules for object files
#-------------------------------------------------------------------------------

list.o: list.c findit.h
	$(CC) $(CFLAGS) -c -o list.o list.c

filter.o: filter.c findit.h
	$(CC) $(CFLAGS) -c -o filter.o filter.c

findit.o: findit.c findit.h
	$(CC) $(CFLAGS) -c -o findit.o findit.c

#-------------------------------------------------------------------------------
# Rules for executables
#-------------------------------------------------------------------------------

findit: filter.o list.o findit.o
	$(LD) $(LDFLAGS) -o findit $^

#-------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#-------------------------------------------------------------------------------

test:
	@$(MAKE) -sk test-all

test-all:	test-gitignore
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-gitignore:
	@echo "findit" > .gitignore
	@echo "*.o" >> .gitignore
	@echo "*.sh" >> .gitignore
	@echo "*.unit" >> .gitignore

test-list:	list.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework08/list.unit.sh
	@chmod +x list.unit.sh
	@./list.unit.sh

list.unit.o:	list.unit.c findit.h
	@$(CC) $(CFLAGS) -c -o $@ $<

list.unit:	list.unit.o list.o
	@$(LD) $(LDFLAGS) -o $@ $^

test-filter:	filter.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework08/filter.unit.sh
	@chmod +x filter.unit.sh
	@./filter.unit.sh

filter.unit.o:	filter.unit.c findit.h
	@$(CC) $(CFLAGS) -c -o $@ $<

filter.unit:	filter.unit.o filter.o
	@$(LD) $(LDFLAGS) -o $@ $^

test-findit:	findit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework08/findit.test.sh
	@chmod +x findit.test.sh
	@./findit.test.sh

clean:
	@rm -f *.o *.sh *.unit findit

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

list_grade, list_status = run_and_extract('make test-list')
filter_grade, filter_status = run_and_extract('make test-filter')
findit_grade, findit_status = run_and_extract('make test-findit')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
all_success = all([
    list_status, filter_status, findit_status, quiz_status
])

print('Grade Summary ...')
print('    List  {:4.2f} /  0.25'.format(0.25*list_grade))
print('  Filter  {:4.2f} /  0.15'.format(0.15*filter_grade))
print('  Findit  {:4.2f} /  0.40'.format(0.40*findit_grade))
print('    Quiz  {:4.2f} /  0.20'.format(0.20*quiz_grade))
print('   Total  {:4.2f} /  1.00'.format(sum([
    0.25*list_grade,
    0.15*filter_grade,
    0.40*findit_grade,
    0.20*quiz_grade
])))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
