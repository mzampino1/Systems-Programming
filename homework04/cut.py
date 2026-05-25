#!/usr/bin/env python3

''' cut.py - remove sections from each line of stream '''

import io
import sys

# Functions

def usage(exit_status: int=0) -> None:
    ''' Print usage message and exit. '''
    print('''Usage: cut -d DELIMITER -f FIELDS

Print selected parts of lines from stream to standard output.

    -d DELIMITER    Use DELIM instead of TAB for field delimiter
    -f FIELDS       Select only these fields''', file=sys.stderr)
    sys.exit(exit_status)

def strs_to_ints(strings: list[str]) -> list[int]:
    ''' Convert all strings in list to integers.

    >>> strs_to_ints(['2', '4'])
    [2, 4]
    '''
    new_list = []
    for str in strings:
        new_list.append(int(str))
    return new_list

def cut_line(line: str, delimiter: str='\t', fields: list[int]=[]) -> list[str]:
    ''' Return selected fields from line separated by delimiter.

    >>> cut_line('Harder, Better, Faster, Stronger', ',', [2, 4])
    [' Better', ' Stronger']
    '''
    all_fields = line.split(delimiter) 
    selected_fields = []
    for field_num in fields:
        try:
            selected_fields.append(all_fields[field_num - 1])
        except IndexError:
            pass
    return selected_fields 

def cut_stream(stream=sys.stdin, delimiter: str='\t', fields: list[int]=[]) -> None:
    ''' Print selected parts of lines from stream to standard output.

    >>> cut_stream(io.StringIO('Harder, Better, Faster, Stronger'), ',', [2, 4])
     Better, Stronger
    '''
    for line in stream:
        line = line.rstrip()
        selected_fields = cut_line(line, delimiter, fields)
        print(delimiter.join(selected_fields))
        

# Main Execution

def main(arguments=sys.argv[1:], stream=sys.stdin) -> None:
    ''' Print selected parts of lines from stream to standard output.

    This function will parse the command line arguments to determine the
    delimiter and which fields to select from each line.

    >>> main('-d , -f 2,4'.split(), io.StringIO('Harder, Better, Faster, Stronger'))
     Better, Stronger
    '''
    # Parse command line arguments
    fields = []

    while arguments:
        argument = arguments.pop(0)
        match argument:
            case '-d': 
                delimiter = arguments.pop(0)
            case '-f':
                field_str = arguments.pop(0)
                fields = field_str.split(',')
                fields = strs_to_ints(fields)
            case '-h':
                usage(0)
            case _: 
                usage(1)
    # No fields specified
    if len(fields) == 0:
        usage(1)

    # Cut stream with delimiter and fields
    cut_stream(stream, delimiter, fields)

if __name__ == '__main__':
    main()

# vim: set sts=4 sw=4 ts=8 expandtab ft=python:
