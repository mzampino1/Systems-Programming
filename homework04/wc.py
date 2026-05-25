#!/usr/bin/env python3

''' wc.py - print newline, word, and byte counts for stream '''

import io
import sys

# Functions

def usage(exit_status: int=0) -> None:
    ''' Print usage message and exit. '''
    print('''Usage: wc.py [-l | -w | -c]

Print newline, word, and byte counts from standard input.

The options below may be used to select which counts are printed, always in
the following order: newline, word, byte.

    -c      Print byte counts
    -l      Print newline counts
    -w      Print word counts''', file=sys.stderr)
    sys.exit(exit_status)

def count_stream(stream=sys.stdin) -> dict[str, int]:
    ''' Count the newlines, words, and bytes in specified stream.

    >>> count_stream(io.StringIO('Despite all my rage, I am still just a rat in a cage'))
    {'newlines': 1, 'words': 13, 'bytes': 52}
    '''
    counts: dict[str, int] = {}
    for line in stream:
        counts['newlines'] = counts.get('newlines', 0) + 1

        words = line.split()
        counts['words'] = counts.get('words', 0) + len(words)
        
        counts['bytes'] = counts.get('bytes', 0) + len(line)

    return counts

def print_counts(counts: dict[str, int], options: list[str]) -> None:
    ''' Print the newline, word, and byte counts.  If none of the options are
    specified, then include all options in output.  Othewrise, only include the
    specified options.

    Note: always output the counts the following order: newlines, words, bytes.

    >>> print_counts({'newlines': 1, 'words': 13, 'bytes': 52}, ['newlines', 'words', 'bytes'])
     1 13 52
    '''

    # Make list of counts and update width for printing
    count_width = 0
    count_list = []

    newline_cnt = counts['newlines']
    if 'newlines' in options or len(options) == 0:
        count_list.append(str(newline_cnt))
    if len(str(newline_cnt)) > count_width:
        count_width = len(str(newline_cnt))

    word_cnt = counts['words']
    if 'words' in options or len(options) == 0:
        count_list.append(str(word_cnt))
    if len(str(word_cnt)) > count_width:
        count_width = len(str(word_cnt))

    byte_cnt = counts['bytes']
    if 'bytes' in options or len(options) == 0:
        count_list.append(str(byte_cnt))
    if len(str(byte_cnt)) > count_width:
        count_width = len(str(byte_cnt))

    # Make width uniform and print counts
    if len(count_list) == 1:
        count_width = 0
    for i in range(len(count_list)):
        count_list[i] = count_list[i].rjust(count_width)
    print(' '.join(count_list))

# Main Execution

def main(arguments=sys.argv[1:], stream=sys.stdin) -> None:
    ''' Print the newline, word, and byte counts from stream.

    This function will parse the command line arguments to select which counts
    to include in the final report.

    >>> main([], io.StringIO('Despite all my rage, I am still just a rat in a cage'))
     1 13 52
    '''
    # Parse command line arguments
    options = []
    while arguments:
        argument = arguments.pop(0)
        match argument:
            case '-c': options.append('bytes')
            case '-l': options.append('newlines')
            case '-w': options.append('words')
            case '-h': usage(0)
            case _: usage(1)

    # Count stream and print counts
    counts = count_stream(stream)
    print_counts(counts, options)

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
