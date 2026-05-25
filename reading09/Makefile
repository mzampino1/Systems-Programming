CC=             gcc
CFLAGS=         -Wall -std=gnu99 -g
TARGETS=        doit

all:            $(TARGETS)

#------------------------------------------------------------------------------- 
# TODO: Add rule for doit
#------------------------------------------------------------------------------- 

doit: doit.c
	$(CC) $(CFLAGS) -o doit doit.c

#------------------------------------------------------------------------------- 
# DO NOT MODIFY BELOW
#-------------------------------------------------------------------------------

test:
	        @$(MAKE) -sk test-all

test-all:	$(TARGETS)
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-doit:	doit
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading09/doit.test.sh
	chmod +x doit.test.sh
	./doit.test.sh

clean:
	rm -f $(TARGETS)

.SUFFIXES:

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

quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
doit_grade, doit_status = run_and_extract('make test-doit')
all_success = all([doit_status, quiz_status])

print('Grade Summary ...')
print('    Quiz  {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('    Doit  {:4.2f} /  0.80'.format(0.8*doit_grade))
print('   Total  {:4.2f} /  1.00'.format(
    0.8*doit_grade + 0.2*quiz_grade
))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
