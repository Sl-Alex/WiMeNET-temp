/**
* @file wmn_queue.c
* @brief Simple queue implementation using circular buffer
* @todo Implement write to the begining of the queue
* @author Oleksii Slabchenko, alexslabchenko@gmail.com
*/

#include "wmn_queue.h"

/// Increment index of the circular buffer
#define INC_INDEX(x)    x = ((x == (WMN_CONFIG_QUEUE_SIZE - 1)) ? 0: x + 1)

/// @brief Initialize queue
/// @param queue - Queue to initialize
void wmn_queue_init(wmn_queue_t * queue)
{
	queue->read_index = 0;
	queue->write_index = 0;
	queue->flags = 0;
	queue->count = 0;
}

/// @brief Write packet to the queue
/// @param queue - queue to write to
/// @param packet - packet to read from
/// @param in_irq - should be non-zero if the function is called from the interrupt handler
/// @returns 0 - read failure (nothing to read)
/// @returns 1 - successful read
uint8_t wmn_queue_write(wmn_queue_t * queue, WmnPacket * packet, uint8_t in_irq)
{
    uint8_t ret = 0;
    in_irq = 0;
    if (in_irq == 0) asm("cpsid i");
	// Clear EMPTY flag
	queue->flags &= ~WMN_QUEUE_FLAG_IS_EMPTY;
	// If we can add new data - do it!
	if (queue->count < WMN_CONFIG_QUEUE_SIZE)
	{
		queue->count++;
		if (queue->count == WMN_CONFIG_QUEUE_SIZE)
			queue->flags |= WMN_QUEUE_FLAG_IS_FULL;
        /// @todo Check assignment
        queue->data[queue->write_index] = *packet;
		INC_INDEX(queue->write_index);
        ret = 1;
	}
    if (in_irq == 0) asm("cpsie i");
	return ret;
}

/// @brief Read packet from the queue
/// @param queue - queue to read from
/// @param packet - packet to write to
/// @param in_irq - should be non-zero if the function is called from the interrupt handler
/// @returns 0 - read failure (nothing to read)
/// @returns 1 - successful read
uint8_t wmn_queue_read(wmn_queue_t * queue, WmnPacket * packet, uint8_t in_irq)
{
    uint8_t ret = 0;
    in_irq = 0;
    if (in_irq == 0) asm("cpsid i");
	if (queue->count > 0)
	{
		queue->count--;
		if (queue->count == 0)
			queue->flags |= WMN_QUEUE_FLAG_IS_EMPTY;
		/// @todo Check assignment
        *packet = queue->data[queue->read_index];
        INC_INDEX(queue->read_index);
		ret = 1;
	}
    if (in_irq == 0) asm("cpsie i");
	return ret;
}
