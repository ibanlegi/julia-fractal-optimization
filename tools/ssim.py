# python3 -m tools.ssim -r export-pictures/jpeg/[reference.jpeg] -t export-pictures/jpeg/[test.jpeg]

from skimage.metrics import structural_similarity as ssim
import cv2
import argparse

def main():
    parser = argparse.ArgumentParser(description="Compute SSIM between two images.")
    parser.add_argument("-r", "--ref" , required=True, help="Reference image path")
    parser.add_argument("-t", "--test", required=True, help="Test image path")
    args = parser.parse_args()

    # Load images
    ref = cv2.imread(args.ref)
    test = cv2.imread(args.test)

    if ref is None:
        raise FileNotFoundError(f"Unable to load reference image : {args.ref}")
    if test is None:
        raise FileNotFoundError(f"Unable to load test image : {args.test}")

    # Conversion to greyscale
    ref_gray = cv2.cvtColor(ref, cv2.COLOR_BGR2GRAY)
    test_gray = cv2.cvtColor(test, cv2.COLOR_BGR2GRAY)

    # SSIM calculation
    score, diff = ssim(ref_gray, test_gray, full=True)

    ssim_percent = score * 100
    print(f"SSIM score : {ssim_percent:.5f} %")

if __name__ == "__main__":
    main()
