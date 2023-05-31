import threading
from multiprocessing import Process, Queue
from multiprocessing.managers import BaseManager


def ue(q: Queue, address: str, port: int, authkey: bytes):
    manager = BaseManager((address, port), authkey)
    manager.register('q', callable=lambda: q)
    manager.get_server().serve_forever()


def main():
    # p = Process(target=ue, args=(queue, address, port, authkey), daemon=True)
    p = threading.Thread(target=ue, args=(queue, address, port, authkey), daemon=True)
    p.start()



if __name__ == '__main__':
    queue, address, port, authkey = Queue(), '', 50000, bytes()
    main()

    queue.put('hello')
    print(queue.get())
    print(queue.get())