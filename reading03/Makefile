test:
	@$(MAKE) -sk test-all

test-all:	test-scripts test-filters

test-scripts:
	curl -sLO https://pnutz.h4x0r.space/courses/cse.20289.sp26/static/txt/reading03/test_filters.sh
	chmod +x test_filters.sh

test-filters:
	./test_filters.sh
