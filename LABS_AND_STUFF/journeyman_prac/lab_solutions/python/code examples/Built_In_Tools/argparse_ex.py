from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument('-f', '--file',
                    dest='filename',
                    help='Write to FILE',
                    metavar='FILE')
parser.add_argument('-q',
                    action='store_false',
                    dest='verbose',
                    default=True,
                    help='Suppress output')
                    
args = parser.parse_args()
print(args.filename, args.verbose)