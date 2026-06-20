Part 1 README.md:


Part 1 of this project consisted of implementing a circular buffer in user space and kernel space.
I began by implementing it in user space, where I included the buffer.h header file and statically initialized the buffer so that
it would retain its value between future calls in buffer_user.c. After doing this, I began defining the prototype functions
provided by buffer.h that dealt with the buffer.

The first prototype function I defined was init_buffer_421(). In order to see if the buffer had already been initialized, I checked
if buffer.read || buffer.write were not NULL. If this is the case, then it means it has been initialized. Furthermore, if that is not
the case, I initialized buffer.length to 0 and began making the linked list. I allocated data for the first node using malloc and
initialized its data to 0. I made the head node be the curr node we just allocated and iterated 1 less than SIZE_OF_BUFFER times,
allocating the next of the curr node and initializing its data to be 0. Furthermore, I used curr = curr->next to iterate through the
linked list. After initializing every node in the buffer, I made the last node's next pointer point to the head to make it a circular
buffer and I also set the buffer.read and buffer.write to be the head as well as a safe starting point. To ensure that the buffer
initialization worked correctly, I checked to see if buffer.read and buffer.write were head and returned 0 for a successful initialization.

Moreover, the next function I defined was insert_buffer_421(int i). This function takes an integer from the user, places it into
the write node's data attribute, and sets the write node to its next node. I implemented this function by first checking to ensure we are not
attempting to insert into an uninitialized or deleted buffer. If buffer.read && buffer.write are both NULL, then it has been uninitialized
or deleted. Finally, if the length of the buffer is SIZE_OF_BUFFER, then it cannot be inserted into because it is full. Both of those
scenarios return -1. However, if those conditions are not met, then the buffer can accept data. This is done by setting buffer.write->data
to the parameter int i, setting the new buffer.write to be the current write node's next node, and then incrementing the length.

In addition, the next function I defined was print_buffer_421(). Similarly to the previous function, I checked to see if the buffer
was uninitialized or deleted by evaluating buffer.read && buffer.write and ensuring that they are both NULL. If that is the case, then
we return -1, and if it is initialized, we continue to print the data of each node. This is done by iterating through the buffer for
SIZE_OF_BUFFER, printing buffer.read->data, and then updating buffer.read to buffer.read->next, thus iterating to the next read node.

Lastly, the delete function was defined by ensuring that the buffer is initialized. If not, then it returns -1. If it is initialized, then
a for loop loops through the buffer for SIZE_OF_BUFFER and frees each node. Once this is complete, it sets the buffer.read and
buffer.write node to NULL and buffer.length to 0.

Nevertheless, that was just the implementation of the user space code and the kernel code of the circular buffer took a little more work.
I included the Linux headers in the buffer.c files and changed the function headers to SYSCALL_DEFINE([# of PARAMETERS])([Name of Fxn])
and changed the corresponding mallocs to kmallocs, printf to printk, and free to kfree. I also had to declare int i
prior to using it for the for loop. I had to include the functions in syscalls, the syscall table, and the directory in the makefile. Once I did this, I
would back out to the cmsc421-project3 directory, run make -j4 bindeb-pkg, and check for errors. Once it compiled, I installed the kernel with
sudo dpkg -i [build name], rebooted, and ran the kernel. I used uname -a to ensure it worked upon reentering the terminal and proceeded to
copy test_user.c to test.c and change the function calls to syscalls(__NR_[function name]). Thus, it worked the same and used sudo dmesg to view
the output.


