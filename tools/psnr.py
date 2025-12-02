# python3 -m tools.psnr -r export-pictures/jpeg/[reference.jpeg] -t export-pictures/jpeg/[test.jpeg]

import cv2
import sys
import argparse

def main():
    parser = argparse.ArgumentParser(description="Compute PSNR between two images.")
    parser.add_argument("-r", "--ref" , required=True, help="Reference image path")
    parser.add_argument("-t", "--test", required=True, help="Test image path")
    args = parser.parse_args()

    ref = cv2.imread(args.ref)
    test = cv2.imread(args.test)

    if ref is None:
        raise FileNotFoundError(f"Unable to load reference image : {args.ref}")
    if test is None:
        raise FileNotFoundError(f"Unable to load test image : {args.test}")

    # Convert to same size if necessary
    if ref.shape != test.shape:
        print("Warning: Images have different sizes. Resizing test image.")
        test = cv2.resize(test, (ref.shape[1], ref.shape[0]))

    psnr_value = cv2.PSNR(ref, test)
    print(f"PSNR : {psnr_value:.4f} dB")

if __name__ == "__main__":
    main()
