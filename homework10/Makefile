CC=		gcc
CFLAGS=		-Wall -std=gnu99 -g
LD=		gcc
LDFLAGS=	-L.
TARGETS=	nmapit curlit

all:		$(TARGETS)

#------------------------------------------------------------------------------
# Rules for object files and executables
#------------------------------------------------------------------------------

socket.o: socket.c socket.h
	$(CC) $(CFLAGS) -c -o socket.o socket.c

nmapit.o: nmapit.c socket.h
	$(CC) $(CFLAGS) -c -o nmapit.o nmapit.c

curlit.o: curlit.c socket.h
	$(CC) $(CFLAGS) -c -o curlit.o curlit.c

nmapit: nmapit.o socket.o
	$(LD) $(LDFLAGS) -o nmapit nmapit.o socket.o

curlit: curlit.o socket.o
	$(LD) $(LDFLAGS) -o curlit curlit.o socket.o

#------------------------------------------------------------------------------
# DO NOT MODIFY BELOW
#------------------------------------------------------------------------------

test:
	@$(MAKE) -sk test-all

test-all:	test-gitignore
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-gitignore:
	@echo "nmapit" > .gitignore
	@echo "curlit" > .gitignore
	@echo "*.o" >> .gitignore
	@echo "*.sh" >> .gitignore
	@echo "*.unit" >> .gitignore

test-socket:	socket.unit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework10/socket.unit.sh
	@chmod +x socket.unit.sh
	@./socket.unit.sh

socket.unit:	socket.unit.c socket.c
	$(CC) $(CFLAGS) -o $@ $^

test-nmapit:	nmapit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework10/nmapit.test.sh
	@chmod +x nmapit.test.sh
	@./nmapit.test.sh

test-curlit:	curlit
	@curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework10/curlit.test.sh
	@chmod +x curlit.test.sh
	@./curlit.test.sh

clean:
	@rm -f $(TARGETS) *.o *.sh *.unit

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

socket_grade, socket_status = run_and_extract('make test-socket')
nmapit_grade, nmapit_status = run_and_extract('make test-nmapit')
curlit_grade, curlit_status = run_and_extract('make test-curlit')
quiz_grade, quiz_status = run_and_extract('../.scripts/check.py')
all_success = all([
    socket_status, nmapit_status, curlit_status, quiz_status
])

print('Grade Summary ...')
print('  Socket  {:4.2f} /  0.10'.format(0.10*socket_grade))
print('  Nmapit  {:4.2f} /  0.30'.format(0.30*nmapit_grade))
print('  Curlit  {:4.2f} /  0.40'.format(0.40*curlit_grade))
print('    Quiz  {:4.2f} /  0.20'.format(0.20*quiz_grade))
print('   Total  {:4.2f} /  1.00'.format(sum([
    0.10*socket_grade,
    0.30*nmapit_grade,
    0.40*curlit_grade,
    0.20*quiz_grade
])))
print('  Status {}'.format('Success' if all_success else 'Failure'))
exit(0 if all_success else 1)
endef

export SUMMARIZE_GRADE_PY
