# I only wrote the search engine. This compiler was completely vibe coded.

import xml.etree.ElementTree as ET
import struct

# CONFIG
XML_FILE = "./data/raw/fra-eng.tei"
OUTPUT_BIN = "./data/compiled/fra-eng.db"
ENTRY_FORMAT = "32s96s" # 32 bytes French, 96 bytes English

print("Parsing XML... this might take a few seconds.")
tree = ET.parse(XML_FILE)
root = tree.getroot()

entries = []

# The TEI namespace used by FreeDict
ns = "{http://www.tei-c.org/ns/1.0}"

for entry in root.findall(f'.//{ns}entry'):
    orth = entry.find(f'.//{ns}orth')
    
    # Ensure the orth tag actually exists and has text
    if orth is not None and orth.text:
        fr = orth.text.lower().strip()
        
        # Find ALL quotes (senses/translations) for this specific word
        quotes = entry.findall(f'.//{ns}quote')
        
        for quote in quotes:
            if quote.text:
                en = quote.text.strip()
                
                # Pack into fixed-width binary records
                # Encode to utf-8 and truncate if too long to prevent struct errors
                entries.append((fr.encode('utf-8')[:31], en.encode('utf-8')[:95]))

# CRITICAL: Binary search requires a sorted file.
# This naturally groups all identical French words together.
print("Sorting binary records...")
entries.sort()

print("Writing to disk...")
with open(OUTPUT_BIN, 'wb') as f:
    for fr, en in entries:
        f.write(struct.pack(ENTRY_FORMAT, fr, en))

print(f"SUCCESS: Wardwordward.db created with {len(entries)} total entries.")
