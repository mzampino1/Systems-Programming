#!/usr/bin/env python3

from typing import Iterator
import io
import sys

# Functions

def odds(stream=sys.stdin) -> list[int]:
    ''' Return a list of all the odd numbers in stream. 

    >>> odds(io.StringIO('\\n'.join('1 2 3 4 5'.split())))
    [1, 3, 5]
    '''
    results = []
    for line in stream:
        number = int(line)
        if number % 2:
            results.append(number)
    return results

def odds_fp(stream=sys.stdin) -> Iterator[int]:
    ''' Return a sequence of odd numbers from stream using map and filter. 

    >>> odds_fp(io.StringIO('\\n'.join('1 2 3 4 5'.split()))) # doctest: +ELLIPSIS
    <filter object at ...>
    '''
    nums = map(int, stream)
    return filter(lambda num: num % 2, nums)

def odds_lc(stream=sys.stdin) -> list[int]:
    ''' Return a list of all the odd numbers in stream using a list
    comprehension.
    
    >>> odds_lc(io.StringIO('\\n'.join('1 2 3 4 5'.split())))
    [1, 3, 5]
    '''
    return [int(num) for num in stream if int(num) % 2]

def odds_gr(stream=sys.stdin) -> Iterator[int]:
    ''' Return a sequence of odd numbers from stream using yield. 
    
    >>> odds_gr(io.StringIO('\\n'.join('1 2 3 4 5'.split()))) # doctest: +ELLIPSIS
    <generator object odds_gr at ...>
    '''
    for num in stream:
        if int(num) % 2:
            yield int(num)

# Main Execution

def main(arguments=sys.argv[1:], stream=sys.stdin) -> None:
    odds_functions = {
        '-f': odds_fp,
        '-l': odds_lc,
        '-g': odds_gr,
    }

    if not arguments:
        print('Usage: odds.py [-f -l -g]', file=sys.stderr)
        sys.exit(1)

    odds_function = odds_functions.get(arguments[0], odds)
    for odd in odds_function(stream):
        print(odd)

if __name__ == '__main__':
    main()
