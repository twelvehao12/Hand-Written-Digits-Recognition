from PySide2.QtWidgets import QFileDialog
from cv2 import imdecode, resize, imshow, waitKey, destroyAllWindows, INTER_CUBIC, IMREAD_GRAYSCALE
from cv2.dnn import readNetFromONNX, blobFromImage
from numpy import fromfile as fromfile
from numpy import uint8, argmax


DEBUG = True


def _run_model_eval(modelpath, impath):
    net = readNetFromONNX(modelpath)
    image = imdecode(fromfile(impath, dtype=uint8), IMREAD_GRAYSCALE)
    image = resize(image, (28, 28))

    ##### test
    # from cv2 import imshow
    # print(image)
    # imshow("gray", image)
    ##### end

    blob = blobFromImage(
        image,
        scalefactor=1,
        size=(28, 28)
    )
    blob /= 255
    net.setInput(blob)
    out = net.forward()
    print(out)
    out = argmax(out, axis=1)
    return out


def file_browse_clicked(ui):
    file_dialog = QFileDialog(ui)
    file_dialog.setFileMode(QFileDialog.AnyFile)
    file_dialog.setViewMode(QFileDialog.Detail)
    filename = file_dialog.getOpenFileName(ui, "选择一张图片")
    ui.image_path_line.setText(filename[0])
    print(filename)


def show_image_button_clicked(ui):
    filepath = ui.image_path_line.text()

    ##### test
    # QMessageBox.about(
    #     ui, 
    #     'test',
    #     filepath
    # )
    # print(filepath)
    ##### end

    # pic = imread(filepath)
    pic = imdecode(fromfile(filepath, dtype=uint8), -1) # 解决中文符号问题
    pic = resize(pic, (512, 512), interpolation=INTER_CUBIC)
    
    imshow("Selected Image(Press any key to exit)", pic)
    waitKey(0)
    destroyAllWindows()



def run_button_clicked(ui):
    filepath = ui.image_path_line.text()

    ##### test
    # ui.result_text.setText(filepath)
    ##### end

    if DEBUG:
        import os
        modelpath = os.path.realpath(os.curdir)+'/digit-rec/model/ResNet18-digits.onnx'
    else:
        modelpath = './model/ResNet18-digits.onnx'
    out = _run_model_eval(modelpath, filepath)

    ##### test
    # print(out)
    ##### end

    ui.result_text.setText(str(out[0]))
