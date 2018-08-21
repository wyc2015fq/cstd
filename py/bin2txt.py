
import os

def read_data_from_binary_file(filename, list_data):
    f = open(filename, 'rb')
    f.seek(0, 0)
    while True:
        t_byte = f.read(1)
        if len(t_byte) == 0:
            break
        else:
            list_data.append("0x%.2X" % ord(t_byte))

def write_data_to_text_file(f_output, list_data,data_num_per_line):
    #f_output.write('')
    if ((data_num_per_line <= 0) or data_num_per_line > len(list_data)):
        data_num_per_line = 16
        print('data_num_per_line out of range,use default value\n')
    for i in range(0,len(list_data)):
        f_output.write(list_data[i]+',')
        if 0==(i+1)%data_num_per_line:
            f_output.write('\n')



if 0:
    list_data = []
    input_f = raw_input("Please input source bin file_name:")
    output_f = raw_input("Please input dest C file name:")
    data_num_per_line = input("Please input a num whitch indicates how many data for one line:")
    read_data_from_binary_file(input_f, list_data)
    write_data_to_text_file(output_f, list_data,data_num_per_line )

def file2txt(input_f, f_output, data_num_per_line):
    list_data = []
    read_data_from_binary_file(input_f, list_data)
    write_data_to_text_file(f_output, list_data,data_num_per_line )

def dir2txt(input_dir, extlist, f_output, data_num_per_line):
    for (root, dirs, files) in os.walk(input_dir):
        for filenameext in files:
            (filename,extension) = os.path.splitext(filenameext)
            if extension in extlist:
                input_f = os.path.join(root,filenameext)
                list_data = []
                read_data_from_binary_file(input_f, list_data)
                f_output.write('unsigned char ' + filename + '[] = {\n')
                write_data_to_text_file(f_output, list_data,data_num_per_line )
                f_output.write('};\n')


if __name__ == "__main__":
    input_f = 'D:/code/git/directui_src/App/res/icons16.bmp'
    output_f = 'D:/code/git/directui_src/App/res/icons16.txt'
    data_num_per_line = 1
    f_output = open(output_f, 'w+')
    file2txt(input_f, output_f, data_num_per_line)
    f_output.close()
