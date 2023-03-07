package main

type FileEx struct {
	file string
	md5  string

	key []byte
}

func NewFile(file, md5 string) *FileEx {
	f := &FileEx{file: file, md5: md5}
	if len(md5) >= 32 {
		f.key = []byte(md5)[:32]
	} else if len(md5) >= 24 && len(md5) < 32 {
		f.key = []byte(md5)[:24]
	} else if len(md5) >= 16 && len(md5) < 24 {
		f.key = []byte(md5)[:16]
	} else {
		f.key = make([]byte, 16)
		copy(f.key, []byte(md5))
	}
	return f
}

func (f *FileEx) Enc(out string) error {
	// in, err := os.Open(f.file)
	// if err != nil {
	// 	return errors.WithMessagef(err, "Enc Open file:%s failed", f.file)
	// }

	// defer in.Close()

	if out == "" {
		out = f.file + ".bin"
	}

	// ff, err := os.Create(out)
	// if err != nil {
	// 	return errors.WithMessagef(err, "Enc Open file:%s failed", out)
	// }

	// defer ff.Close()

	return EncryptFile(f.file, out, f.key)
}

func (f *FileEx) Dec(out string) error {

	if out == "" {
		out = f.file + ".txt"
	}
	return DecryptFile(f.file, out, f.key)
}
