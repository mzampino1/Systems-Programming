test:
	@$(MAKE) -sk test-all

test-all:	test-scripts
	python3 -c "$$SUMMARIZE_GRADE_PY"

test-scripts:
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework03/test_weather.sh
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/homework03/test_zipcode.sh
	chmod +x test_*.sh

test-weather:
	./test_weather.sh

test-zipcode:
	./test_zipcode.sh

test-quiz:
	../.scripts/check.py

# Embedded Grade Summary Python Script

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

weather_grade, weather_status = run_and_extract('./test_weather.sh')
zipcode_grade, zipcode_status = run_and_extract('./test_zipcode.sh')
quiz_grade   , quiz_status    = run_and_extract('../.scripts/check.py')

print('Grade Summary ...')
print('  Weather {:4.2f} /  0.40'.format(0.4*weather_grade))
print('  Zipcode {:4.2f} /  0.40'.format(0.4*zipcode_grade))
print('  Quiz    {:4.2f} /  0.20'.format(0.2*quiz_grade))
print('  Total   {:4.2f} /  1.00'.format(
    0.4*weather_grade + 0.4*zipcode_grade + 0.2*quiz_grade
))
print('  Status {}'.format(
    'Success' if all([weather_status, zipcode_status, quiz_status]) else 'Failure'
))
endef

export SUMMARIZE_GRADE_PY
