#!/usr/bin/env python3

import sys, os

exit_code = 0

# Check if each argument exists
for arg in sys.argv[1:]:
    if os.path.exists(arg):
        print(f"{arg} exists!")
    else:
        print(f"{arg} does not exist!")
        exit_code = 1

# Return appropriate exit code
sys.exit(exit_code)
