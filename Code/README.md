## InterFaceGan

Run the script `pretrained_model.py`

Once you have completed the setup, execute the `edit.py` script :

```bash
python3 edit.py \
    -m pggan_celebahq \
    -b boundaries/pggan_celebahq_gender_boundary.npy \
    -n 1000 \
    -o results/pggan_celebahq_gender_editing
```
This script is designed to generate 1000 gender modifications.

You can now run the `swap_gender.py` script :
```bash
python3 swap_gender.py \
    path to image\
    gender (h or f) \
    path to images generated \
```

