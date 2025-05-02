#include "middle_event_process.h"

#include <stdint.h>
#include <string.h>

#include "component/log/my_log.h"

static EVENT_t eventQueue[EVENT_QUEUE_SIZE];
static uint8_t eventQueueHead = 0;
static uint8_t eventQueueTail = 0;

static EVENT_t m_cbs[EVENT_QUEUE_SIZE];  // �¼����б�

void registCallback(EVENT_TYPE type, event_callback cb) {
  m_cbs[type].type     = type;
  m_cbs[type].callback = cb;
}

void enterQueueEvent(EVENT_TYPE type) {
  if ((eventQueueHead + 1) % EVENT_QUEUE_SIZE == eventQueueTail) {
    LOGW("event queue full");
    return;
  }
  eventQueue[eventQueueHead].type     = type;
  eventQueue[eventQueueHead].callback = m_cbs[type].callback;
  eventQueueHead                      = (eventQueueHead + 1) % EVENT_QUEUE_SIZE;
}

void process_event(void) {
  while (eventQueueTail != eventQueueHead) {
    // LOGI("EVENT PROCESS");
    EVENT_t event = eventQueue[eventQueueTail];
    if (event.callback != NULL) {
      event.callback();  // ���ûص�����
    }
    eventQueueTail = (eventQueueTail + 1) % EVENT_QUEUE_SIZE;
  }
}

// #include "module_driver/driver_wireless.h"

// static BOOK_DATA_t book_database[128];
// static uint8_t book_database[][20];

// static EVENT_t event;
// static uint8_t index_event;

// void attach_queue(ENUM_DATA_PACK_EVENT h_event) {
//   // event
//   if (index_event >= EVENT_NUM) {
//     LOGE("event num overflow is :%d", index_event);
//     return;
//   }

//   if (h_event == NULL) {
//     LOGE("event ERROR");
//     return;
//   }

//   event.event_queue[index_event] = h_event;
//   index_event++;
// }

// void detect_queue(ENUM_DATA_PACK_EVENT h_event) {
//   // event
//   uint8_t queue_pos;
//   if (index_event <= 0) {
//     LOGE("event empty is :%d", index_event);
//     return;
//   }
//   if (h_event == NULL) {
//     LOGE("event ERROR");
//     return;
//   }

//   for (queue_pos = 0; queue_pos < index_event; queue_pos++) {
//     if (event.event_queue[queue_pos] == h_event) {
//       if (queue_pos == index_event - 1) {
//         event.event_queue[queue_pos] = kPackIdle;
//         index_event--;
//         LOGI("in event queue end ");
//         return;
//       } else {
//         for (int j = queue_pos; j < index_event; j++) {
//           event.event_queue[j] = event.event_queue[j + 1];
//         }
//         LOGI("in event queue : %d ", queue_pos);
//         index_event--;
//         return;
//       }
//     }
//   }
//   LOGE("type not exsit");
// }

// void event_process(void) {
//   if (0 == index_event) {
//     return;
//   }

//   event.type = event.event_queue[0];
//   detect_queue(event.event_queue[0]);

//   switch (event.type) {
//     case kPackIdle:
//       /* code */
//       break;

//     case kPicData:
//       pic_data_process();
//       break;
//     case kTrace:
//       break;
//     case kShow:
//       break;
//   }
// }

// void event_data_book(const uint8_t *data, uint8_t len) {
//   uint8_t book_num = len / 14;
//   for (int i = 0; i < book_num; i++) {
//     memcpy(book_database[i], &(data[i * 14]), 14);
//   }
//   LOGI("SUCESS");

//   for (int j = 0; j < 14; j++) {
//     LOGI("%02x", book_database[0][j]);
//   }
// }