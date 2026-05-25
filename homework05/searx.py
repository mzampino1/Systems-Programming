#!/usr/bin/env python3

''' searx.py - SearX from the command line '''

import sys

import requests

# Constants

URL     = 'https://searx.ndlug.org/search'
LIMIT   = 5
ORDERBY = 'score'

# Functions

def usage(exit_status: int=0) -> None:
    ''' Print usage message and exit. '''
    print(f'''Usage: searx.py [-u URL -n LIMIT -o ORDERBY] QUERY

Fetch SearX results for QUERY and print them out.

    -u URL      Use URL as the SearX instance (default is: {URL})
    -n LIMIT    Only display up to LIMIT results (default is: {LIMIT})
    -o ORDERBY  Sort the search results by ORDERBY (default is: {ORDERBY})

If ORDERBY is score, the results are shown in descending order.  Otherwise,
results are shown in ascending order.''', file=sys.stderr)
    sys.exit(exit_status)

def searx_query(query: str, url: str=URL) -> list[dict]:
    ''' Returns lists of results for query from SearX.

    >>> searx_query('Python', 'https://yld.me/ipfM') # doctest: +ELLIPSIS
    [{'url': 'https://www.python.org/', 'title': 'Welcome to Python.org', ...}]
    '''
    params = {'q': query, 'format': 'json'}
    response = requests.get(url, params=params)
    return response.json()['results']

def print_results(results: list[dict], limit: int=LIMIT, orderby: str=ORDERBY) -> None:
    ''' Print results of SearX query.

    >>> print_results(searx_query('Python', 'https://yld.me/ipfM')) # doctest: +ELLIPSIS, +NORMALIZE_WHITESPACE
        1.  Welcome to Python.org [...]
            https://www.python.org/
    ...
    '''
    reverse = True if orderby == 'score' else False
    results = sorted(results, key=lambda r: r[orderby], reverse=reverse) 
    for index, result in enumerate(results, 1):
        # Only print a certain number of results
        if index == limit + 1:
            break

        if index != 1:
            print()

        title = result['title']
        score = result['score']
        url = result['url']
        print(f'{index:>4}.\t{title} [{score:0.2f}]')
        print(f'\t{url}')

# Main Execution

def main(arguments=sys.argv[1:]) -> None:
    ''' Searches SearX and print results.

    >>> main('-u https://yld.me/ipfM Python'.split()) # doctest: +ELLIPSIS, +NORMALIZE_WHITESPACE
        1.  Welcome to Python.org [...]
            https://www.python.org/
    ...
    '''
    url = URL
    limit = LIMIT
    orderby = ORDERBY
    query = ''
    while arguments:
        arg = arguments.pop(0)
        match arg:
            case '-u': url = arguments.pop(0)
            case '-n': limit = int(arguments.pop(0))
            case '-o': orderby = arguments.pop(0)
            case '-h': usage(0)
            case _:
                if arg[0] == '-':
                    usage(1)
                query = arg
                if arguments:
                    query = arg + ' ' + ' '.join(arguments)
                    break
    # Exit with error if no query is given
    if query == '':
        usage(1)

    results = searx_query(query, url)
    print_results(results, limit, orderby)

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
