project('asrrgb', 'cpp', version: '1.1.0')

# Define source files
src_files = ['src/main.cpp', 'src/Asrrgb.cpp', 'src/Cmdline.cpp']

# Define the target executable
my_exe = executable('asrrgb', src_files,
                    include_directories: ['src/include'],
                    dependencies: [dependency('libusb-1.0'),
                                   dependency('boost', modules : ['program_options'])],
					install : true,
					install_dir: 'usr/bin')

# Install udev rules file
install_data('60-asrrgb.rules', install_dir: 'etc/udev/rules.d/')
