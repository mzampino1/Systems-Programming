CC=		gcc
CFLAGS=		-Wall -std=gnu99 -g
LD=		gcc
LDFLAGS=	-L.
TARGETS=	seqit tailit

all:		$(TARGETS)

#-------------------------------------------------------------------------------
# Add rules for node.o, list.o, seqit.o, tailit.o, seqit, tailit
#-------------------------------------------------------------------------------

node.o: node.c list.h
	$(CC) $(CFLAGS) -c -o node.o node.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c -o list.o list.c

seqit.o: seqit.c list.h
	$(CC) $(CFLAGS) -c -o seqit.o seqit.c

tailit.o: tailit.c list.h
	$(CC) $(CFLAGS) -c -o tailit.o tailit.c

seqit: seqit.o node.o list.o
	$(LD) $(LDFLAGS) -o seqit seqit.o node.o list.o

tailit: tailit.o node.o list.o
	$(LD) $(LDFLAGS) -o tailit tailit.o node.o list.o

#-------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#-------------------------------------------------------------------------------

test:
	@$(MAKE) -sk test-all

test-all:       test-gitignore
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-gitignore:
	@echo "*.o" > .gitignore
	@echo "*.sh" >> .gitignore
	@echo "*.unit" >> .gitignore

node.unit:	node.unit.c node.o
	$(LD) $(LDFLAGS) -o node.unit node.unit.c node.o

test-node:	node.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework07/node.unit.sh
	@chmod +x node.unit.sh
	@./node.unit.sh

list.unit:	list.unit.c node.o list.o
	$(LD) $(LDFLAGS) -o list.unit list.unit.c node.o list.o

test-list:	list.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework07/list.unit.sh
	@chmod +x list.unit.sh
	@./list.unit.sh

test-seqit:	seqit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework07/seqit.test.sh
	@chmod +x seqit.test.sh
	@./seqit.test.sh

test-tailit:	tailit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework07/tailit.test.sh
	@chmod +x tailit.test.sh
	@./tailit.test.sh

clean:
	@rm -f *.o *.unit *.unit.sh *.test.sh seqit tailit

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

node_grade, node_status = run_and_extract('make test-node')
list_grade, list_status = run_and_extract('make test-list')
seqit_grade, seqit_status = run_and_extract('make test-seqit')
tailit_grade, tailit_status = run_and_extract('make test-tailit')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
all_success = all([
    node_status, list_status, seqit_status, tailit_status, quiz_status
])

print('Grade Summary ...')
print('  Node    {:4.2f} /  0.10'.format(0.10*node_grade))
print('  List    {:4.2f} /  0.20'.format(0.20*list_grade))
print('  Seqit   {:4.2f} /  0.20'.format(0.20*seqit_grade))
print('  Tailit  {:4.2f} /  0.20'.format(0.20*tailit_grade))
print('  Quiz    {:4.2f} /  0.30'.format(0.30*quiz_grade))
print('  Total   {:4.2f} /  1.00'.format(sum([
    0.10*node_grade , 0.20*list_grade, 
    0.20*seqit_grade, 0.20*tailit_grade, 
    0.30*quiz_grade
])))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
