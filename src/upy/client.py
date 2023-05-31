import json
from multiprocessing import Queue
from multiprocessing.managers import BaseManager

import numpy as np

m = BaseManager(address=('127.0.0.1', 50000), authkey=bytes())
m.connect()

m.register('q')
q: Queue = getattr(m, 'q')()

vertices = np.array([1, 2, 3], dtype=np.float32)
triangles = np.array([1, 2, 3], dtype=np.int32)

q.put({
    'message': json.dumps({'name': 'new_mesh'}, indent=4, ensure_ascii=False),
    'payload': {
        'vertices': vertices.tobytes(),
        'triangles': triangles.tobytes(),
    },
})

# print(to_ue.get(False))
# print(to_ue.get(False))
