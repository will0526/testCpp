gcc -dynamiclib -I /Library/Java/JavaVirtualMachines/jdk-11.0.2.jdk/Contents/Home/include/ splitFile.cpp -o libsplitFile.jnilib

gcc -I/System/Library/Frameworks/JavaVM.framework/Headers -c splitFile.cpp


gcc -dynamiclib -o libsplitfile.jnilib splitFile.o -lstdc++