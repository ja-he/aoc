let s: string = '';
process.stdin.setEncoding('utf8');
process.stdin.on('data', (chunk) => {
  s += chunk;
});
process.stdin.on('end', async () => {
  if (s.length === 0) {
    throw new Error('read too little');
  }

  let sum: number = 0;
  const lines = s.split('\n');
  for (const line of lines) {
    if (line === '') {
      continue;
    }

    const b = Buffer.from(line, 'utf8');
    const indices = getCalibrationValueIndices(b);

    const digits: Uint8Array = new Uint8Array([b[indices[0]], b[indices[1]]]);

    // we concatenate all (two) digits to an ASCII string (of digits)
    const twoDigitNumber = digits.reduce((acc, cur) => acc + String.fromCharCode(cur), '');

    sum += parseInt(twoDigitNumber, 10);
  }

  console.log(sum);
});

function getCalibrationValueIndices(s: Uint8Array): [number, number] {
  let first: number | null = null;
  let last: number | null = null;

  for (let i = 0; i < s.length; i++) {
    if (s[i] >= 48 && s[i] <= 57) {
      first = i;
      break;
    }
  }
  if (first === null) {
    throw new Error('no digits found in string "' + s + '"');
  }

  for (let i = s.length; i >= first; i--) {
    if (s[i] >= 48 && s[i] <= 57) {
      last = i;
      break;
    }
  }
  if (last === null) {
    // we know that this cannot happen
    throw new Error('the string "' + s + '"has only one digit');
  }

  return [first, last];
}
