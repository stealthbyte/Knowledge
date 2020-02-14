import hashlib
import os.path

# import common


def hash_file(file_path):
    hash_obj = hashlib.sha256()
    with open(file_path, "rb") as fh:
        chunk_size = 1024
        bytes_read = fh.read(chunk_size)
        while bytes_read:
            hash_obj.update(bytes_read)
            bytes_read = fh.read(chunk_size)

    return hash_obj.hexdigest()


def hash_folder(directory):
    for my_file in os.listdir(directory):
        full_path = os.path.join(directory, my_file)
        if os.path.isfile(full_path):
            try:
                yield full_path, hash_file(full_path)
            except IOError as ioe:
                if ioe.errno == 13:  # permission denied
                    pass
                else:
                    raise


# def hash_file(file_path):
#    hash_type = hashlib.sha256
#    hash = common.hash_file(file_path, hash_type)
#
#    return hash
#
#
# def hash_folder(directory):
#    hash_type = hashlib.sha256
#
#    for full_path, hash in common.hash_folder(directory, hash_type):
#        yield full_path, hash
