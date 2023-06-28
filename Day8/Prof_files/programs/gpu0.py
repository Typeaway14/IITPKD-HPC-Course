import torch
print(torch.__version__)
print(f"Is available: {torch.cuda.is_available()}")

try:
    print(f"Current Devices: {torch.cuda.current_device()}")
except :
    print('Current Devices: Torch is not compiled for GPU or No GPU')

print(f"No. of GPUs: {torch.cuda.device_count()}")
