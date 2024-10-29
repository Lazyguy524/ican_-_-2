import os
import sys
import numpy as np
import cv2
import json
from typing import Any

My_CLASSES = ("background", "open", "short", "mousebite", "spur", "copper", "pin-hole")

COLORS = [
        [56, 0, 255],
        [226, 255, 0],
        [0, 94, 255],
        [0, 37, 255],
        [0, 255, 94],
        [255, 226, 0],
        [0, 18, 255],
        [255, 151, 0],
        [170, 0, 255],
        [0, 255, 56],
        [255, 0, 75],
        [0, 75, 255],
        [0, 255, 169],
        [255, 0, 207],
        [75, 255, 0],
        [207, 0, 255],
        [37, 0, 255],
        [0, 207, 255],
        [94, 0, 255],
        [0, 255, 113],
        [255, 18, 0],
        [255, 0, 56],
        [18, 0, 255],
        [0, 255, 226],
        [170, 255, 0],
        [255, 0, 245],
        [151, 255, 0],
        [132, 255, 0],
        [75, 0, 255],
        [151, 0, 255],
        [0, 151, 255],
        [132, 0, 255],
        [0, 255, 245],
        [255, 132, 0],
        [226, 0, 255],
        [255, 37, 0],
        [207, 255, 0],
        [0, 255, 207],
        [94, 255, 0],
        [0, 226, 255],
        [56, 255, 0],
        [255, 94, 0],
        [255, 113, 0],
        [0, 132, 255],
        [255, 0, 132],
        [255, 170, 0],
        [255, 0, 188],
        [113, 255, 0],
        [245, 0, 255],
        [113, 0, 255],
        [255, 188, 0],
        [0, 113, 255],
        [255, 0, 0],
        [0, 56, 255],
        [255, 0, 113],
        [0, 255, 188],
        [255, 0, 94],
        [255, 0, 18],
        [18, 255, 0],
        [0, 255, 132],
        [0, 188, 255],
        [0, 245, 255],
        [0, 169, 255],
        [37, 255, 0],
        [255, 0, 151],
        [188, 0, 255],
        [0, 255, 37],
        [0, 255, 0],
        [255, 0, 170],
        [255, 0, 37],
        [255, 75, 0],
        [0, 0, 255],
        [255, 207, 0],
        [255, 0, 226],
        [255, 245, 0],
        [188, 255, 0],
        [0, 255, 18],
        [0, 255, 75],
        [0, 255, 151],
        [255, 56, 0],
        [245, 255, 0],
    ]


def save_data_to_json(file_name, x1, y1, x2, y2, conf, defect_type, output_dir):
    data = {
        "x1": x1,
        "y1": y1,
        "x2": x2,
        "y2": y2,
        "conf": conf,
        "defect_type": defect_type
    }

    # Remove the extension from the file_name
    file_name = os.path.splitext(file_name)[0]
    
    json_file_path = 'position.json'
    output_dir_json = output_dir + "json"

    if not os.path.exists(output_dir_json):
        os.makedirs(output_dir_json)
    
    save_name = os.path.join(output_dir_json, json_file_path)

    # Check if the file exists and is not empty
    if os.path.exists(save_name) and os.path.getsize(save_name) > 0:
        # Append new data to existing JSON array
        with open(save_name, 'r+') as json_file:
            # Read existing data
            existing_data = json.load(json_file)
            # Append new data
            existing_data.append(data)
            # Go to the beginning of the file
            json_file.seek(0)
            # Write the updated data
            json.dump(existing_data, json_file, indent=4)
    else:
        # Create new JSON array with the first data entry
        with open(save_name, 'w') as json_file:
            json.dump([data], json_file, indent=4)

def draw_numpy(image, boxes, masks=None, classes_ids=None, conf_scores=None):
    for idx in range(len(boxes)):
        x1, y1, x2, y2 = boxes[idx, :].astype(np.int32).tolist()
        if classes_ids is not None:
            color = COLORS[int(classes_ids[idx]) + 1]
        else:
            color = (0, 0, 255)
        cv2.rectangle(image, (x1, y1), (x2, y2), color, thickness=1)
        if classes_ids is not None and conf_scores is not None:
            classes_ids = classes_ids.astype(np.int8)
            txt_color = (0, 0, 0) if np.mean(COLORS[idx]) > 0.5 else (255, 255, 255)
            # cv2.putText(image, My_CLASSES[classes_ids[idx]] + ':' + str(round(conf_scores[idx], 2)),
            #             (x1, y1 - 5),
            #             cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), thickness=2)
            cv2.putText(image, My_CLASSES[classes_ids[idx]] + ':' + str(round(conf_scores[idx], 2)),
                        (x1, y1 - 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, color, thickness=2)

        if masks is not None:
            mask = masks[:, :, idx]
            image[mask] = image[mask] * 0.5 + np.array(color) * 0.5
    return image

def draw_numpy_json(image, boxes, masks=None, classes_ids=None, conf_scores=None,file_name=None,output_dir=None):
    for idx in range(len(boxes)):
        x1, y1, x2, y2 = boxes[idx, :].astype(np.int32).tolist()
        if classes_ids is not None:
            color = COLORS[int(classes_ids[idx]) + 1]
        else:
            color = (0, 0, 255)
        cv2.rectangle(image, (x1, y1), (x2, y2), color, thickness=1)
        if classes_ids is not None and conf_scores is not None:
            classes_ids = classes_ids.astype(np.int8)
            txt_color = (0, 0, 0) if np.mean(COLORS[idx]) > 0.5 else (255, 255, 255)
            # cv2.putText(image, My_CLASSES[classes_ids[idx]] + ':' + str(round(conf_scores[idx], 2)),
            #             (x1, y1 - 5),
            #             cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), thickness=2)
            cv2.putText(image, My_CLASSES[classes_ids[idx]] + ':' + str(round(conf_scores[idx], 2)),
                        (x1, y1 - 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 1, color, thickness=2)

            save_data_to_json(file_name,x1,y1,x2,y2,str(round(conf_scores[idx], 2)),My_CLASSES[classes_ids[idx]],output_dir)

        if masks is not None:
            mask = masks[:, :, idx]
            image[mask] = image[mask] * 0.5 + np.array(color) * 0.5
    return image

def draw_bmcv(bmcv, image, boxes, masks=None, classes_ids=None, conf_scores=None):
    for idx in range(len(boxes)):
        x1, y1, x2, y2 = boxes[idx, :].astype(np.int32).tolist()
        if classes_ids is not None:
            color = np.array(COLORS[int(classes_ids[idx]) + 1]).astype(np.uint8).tolist()
        else:
            color = (0, 0, 255)
        bmcv.rectangle(image, x1, y1, (x2 - x1), (y2 - y1), color, 2)
        score = conf_scores[idx]
        text = '{}:{:.1f}%'.format(My_CLASSES[idx], score * 100)
        txt_color = (0, 0, 0) if np.mean(COLORS[idx]) > 0.5 else (255, 255, 255)
        font = cv2.FONT_HERSHEY_SIMPLEX
        txt_size = cv2.getTextSize(text, font, 0.4, 1)[0]
        # bmcv.putText(handle,image,text,{x1,y1},COLORS[idx+10],cv2.FONT_HERSHEY_SIMPLEX,thickness=2)
        # txt_bk_color = (COLORS[idx] * 255 * 0.7).astype(np.uint8).tolist()
        # cv2.putText(image, text, (x1, y1 + txt_size[1]), font, 0.4, txt_color, thickness=1)
        # cv2.putText(image,text,(x1,y1),cv2.FONT_HERSHEY_SIMPLEX,1,(0,255,0),thickness=2)

def is_img(file_name):
    """judge the file is available image or not
    Args:
        file_name (str): input file name
    Returns:
        (bool) : whether the file is available image
    """
    fmt = os.path.splitext(file_name)[-1]
    if isinstance(fmt, str) and fmt.lower() in ['.jpg','.png','.jpeg','.bmp','.jpeg','.webp']:
        return True
    else:
        return False

# scipy: https://zhuanlan.zhihu.com/p/375344904
def logsumexp(a, axis=None, b=None, keepdims=False, return_sign=False):
    """Compute the log of the sum of exponentials of input elements.
    Parameters
    ----------
    a : array_like
        Input array.
    axis : None or int or tuple of ints, optional
        Axis or axes over which the sum is taken. By default `axis` is None,
        and all elements are summed.
        .. versionadded:: 0.11.0
    keepdims : bool, optional
        If this is set to True, the axes which are reduced are left in the
        result as dimensions with size one. With this option, the result
        will broadcast correctly against the original array.
        .. versionadded:: 0.15.0
    b : array-like, optional
        Scaling factor for exp(`a`) must be of the same shape as `a` or
        broadcastable to `a`. These values may be negative in order to
        implement subtraction.
        .. versionadded:: 0.12.0
    return_sign : bool, optional
        If this is set to True, the result will be a pair containing sign
        information; if False, results that are negative will be returned
        as NaN. Default is False (no sign information).
        .. versionadded:: 0.16.0
    Returns
    -------
    res : ndarray
        The result, ``np.log(np.sum(np.exp(a)))`` calculated in a numerically
        more stable way. If `b` is given then ``np.log(np.sum(b*np.exp(a)))``
        is returned.
    sgn : ndarray
        If return_sign is True, this will be an array of floating-point
        numbers matching res and +1, 0, or -1 depending on the sign
        of the result. If False, only one result is returned.

    """
    # a = _asarray_validated(a, check_finite=False)
    if b is not None:
        a, b = np.broadcast_arrays(a, b)
        if np.any(b == 0):
            a = a + 0.  # promote to at least float
            a[b == 0] = -np.inf

    a_max = np.amax(a, axis=axis, keepdims=True)

    if a_max.ndim > 0:
        a_max[~np.isfinite(a_max)] = 0
    elif not np.isfinite(a_max):
        a_max = 0

    if b is not None:
        b = np.asarray(b)
        tmp = b * np.exp(a - a_max)
    else:
        tmp = np.exp(a - a_max)

    # suppress warnings about log of zero
    with np.errstate(divide='ignore'):
        s = np.sum(tmp, axis=axis, keepdims=keepdims)
        if return_sign:
            sgn = np.sign(s)
            s *= sgn  # /= makes more sense but we need zero -> zero
        out = np.log(s)

    if not keepdims:
        a_max = np.squeeze(a_max, axis=axis)
    out += a_max

    if return_sign:
        return out, sgn
    else:
        return out

# scipy.special.softmax: https://zhuanlan.zhihu.com/p/375344904
def softmax(x, axis=None):
    r"""
    Softmax function
    The softmax function transforms each element of a collection by
    computing the exponential of each element divided by the sum of the
    exponentials of all the elements. That is, if `x` is a one-dimensional
    numpy array::
        softmax(x) = np.exp(x)/sum(np.exp(x))
    Parameters
    ----------
    x : array_like
        Input array.
    axis : int or tuple of ints, optional
        Axis to compute values along. Default is None and softmax will be
        computed over the entire array `x`.
    Returns
    -------
    s : ndarray
        An array the same shape as `x`. The result will sum to 1 along the
        specified axis.

    """
    # compute in log space for numerical stability
    return np.exp(x - logsumexp(x, axis=axis, keepdims=True))