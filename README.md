# Check-Off 3: Data Compression

## Topic

For this check-off I explored **data compression**: how to represent data using fewer bytes than the original. I focused on one of the simplest compression algorithms, **Run-Length Encoding (RLE)**, and wrote it in C three times, with each version building on the last.

## What I Explored

### Lossy vs. Lossless Compression

I first learned that there are two main kinds of compression:

- **Lossy compression** throws away some information to make the file smaller, so you can't get the exact original back. This works for things like images (JPEG), audio (MP3), and video, where small differences are hard for people to notice.
- **Lossless compression** keeps all of the information, so decompressing gives back exactly the original data. This is needed for things like text, code, and executables, where changing even one byte would break things (ZIP and PNG are lossless).

RLE is a **lossless** algorithm.

### How RLE Works

RLE replaces a "run" of the same character repeated in a row with a count followed by the character. For example:

```
AAAAAB  ->  5A1B
```

To decompress, you read each `(count, character)` pair and write that character `count` times. RLE works well on data with long runs of repeated values, like simple images with large areas of one color, and badly on data that rarely repeats.

## My Three Code Snippets

### 1. `simple_compression.c`: The Basic Version

This was me getting the basic idea working. It encodes a string like `"AAAAAB"` into readable text (`"5A1B"`) and decodes it back. The count is stored as an ASCII digit (`'0' + count`), so a run can be at most **9** characters long. Anything longer gets split into several runs. This made the algorithm easy to see, but it only works on text and wastes space.

### 2. `normal_compression.c`: The Standardized Version

This version is closer to how RLE is actually done. It works on raw bytes (`unsigned char`) with an explicit length instead of relying on a null terminator, so it can handle any binary data. The count is stored as a raw byte instead of a digit, so a run can go up to **255**. As a test, it compresses 300 `'A'` bytes down to just **4 bytes** (`255 A` followed by `45 A`) and checks that decoding gives back exactly the original.

### 3. `edge_compression.c`: Handling the Edge Case

This version handles the case where RLE makes the data **bigger** instead of smaller. Every run costs 2 bytes, so data without repeats roughly doubles in size. For example, `"HELLO"` (5 bytes) would encode to 8 bytes.

To fix this, `compress()` adds a 1-byte **flag** at the front:

- If the RLE output is smaller than the input, it stores `FLAG_RLE` followed by the encoded data.
- Otherwise, it stores `FLAG_RAW` followed by an unchanged copy of the original.

`decompress()` reads the flag to decide whether to RLE-decode or just copy. So the worst case is now only **1 byte** bigger than the original instead of about double.

## Most Interesting Discovery: The Pigeonhole Principle

The most interesting thing I learned is that **no lossless compression algorithm can make every file smaller**, and the reason is the pigeonhole principle. There are more possible files of length *n* than there are files shorter than *n*. So if an algorithm shrank every input, two different inputs would have to compress to the same output, and you couldn't tell them apart when decompressing. That would make it lossy. This means every lossless compressor must make some inputs the same size or bigger. My flag-based fix in `edge_compression.c` doesn't get around this. It just limits how much bigger a file can get to one byte, which is the best a lossless compressor can do.
