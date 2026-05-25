CC=		gcc
CFLAGS=		-Wall -std=gnu99 -g
TARGETS=	moveit timeit

all:		$(TARGETS)

#------------------------------------------------------------------------------
# Add rules for moveit, timeit
#------------------------------------------------------------------------------

moveit: moveit.c
	$(CC) $(CFLAGS) -o moveit moveit.c

timeit: timeit.c
	$(CC) $(CFLAGS) -o timeit timeit.c

#------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#------------------------------------------------------------------------------

test:
	@$(MAKE) -sk test-all

test-all:	test-gitignore
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-gitignore:
	@echo "moveit" > .gitignore
	@echo "timeit" > .gitignore
	@echo "*.sh" >> .gitignore

test-moveit:	moveit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework09/moveit.test.sh
	@chmod +x moveit.test.sh
	@./moveit.test.sh

test-timeit:	timeit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework09/timeit.test.sh
	@chmod +x timeit.test.sh
	@./timeit.test.sh

clean:
	@rm -f $(TARGETS) *.sh

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

moveit_grade, moveit_status = run_and_extract('make test-moveit')
timeit_grade, timeit_status = run_and_extract('make test-timeit')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
all_success = all([
    moveit_status, timeit_status, quiz_status
])

print('Grade Summary ...')
print('  Moveit  {:4.2f} /  0.40'.format(0.40*moveit_grade))
print('  Timeit  {:4.2f} /  0.40'.format(0.40*timeit_grade))
print('    Quiz  {:4.2f} /  0.20'.format(0.20*quiz_grade))
print('   Total  {:4.2f} /  1.00'.format(sum([
    0.40*moveit_grade,
    0.40*timeit_grade,
    0.20*quiz_grade
])))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
