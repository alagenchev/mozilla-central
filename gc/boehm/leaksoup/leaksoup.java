/* -*- Mode: Java; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express oqr
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, released
 * March 31, 1998.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *
 * Patrick C. Beard <beard@netscape.com>
 *
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU Public License (the "GPL"), in which case the
 * provisions of the GPL are applicable instead of those above.
 * If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use your
 * version of this file under the NPL, indicate your decision by
 * deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this
 * file under either the NPL or the GPL.
 */

import java.io.*;
import java.util.*;

class Type {
	String mName;
	int mSize;

	Type(String name, int size) {
		mName = name;
		mSize = size;
	}

	public int hashCode() {
		return mName.hashCode() + mSize;
	}

	public boolean equals(Object obj) {
		if (obj instanceof Type) {
			Type t = (Type) obj;
			return (t.mSize == mSize && t.mName.equals(mName));
		}
		return false;
	}

	public String toString() {
		return "<A HREF=\"#" + mName + "_" + mSize + "\">&LT;" + mName + "&GT;</A> (" + mSize + ")";
	}

	static class Comparator implements QuickSort.Comparator {
		public int compare(Object obj1, Object obj2) {
			Type t1 = (Type) obj1, t2 = (Type) obj2;
			return (t1.mSize - t2.mSize);
		}
	}
}

class Leak {
	String mAddress;
	Type mType;
	Object[] mReferences;
	Object[] mCrawl;
	int mRefCount;
	Leak[] mParents;
	int mTotalSize;

	Leak(String addr, Type type, Object[] refs, Object[] crawl) {
		mAddress = addr;
		mReferences = refs;
		mCrawl = crawl;
		mRefCount = 0;
		mType = type;
		mTotalSize = 0;
	}
	
	void setParents(Vector parents) {
		mParents = new Leak[parents.size()];
		parents.copyInto(mParents);
	}
	
	void computeTotalSize() {
		// first, mark this node as having been visited.
		// we only want to include nodes that haven't been
		// visited in our total size.
		mTotalSize = mType.mSize;
		
		// then, visit all nodes that haven't been visited,
		// and include their total size in ours.
		int count = mReferences.length;
		for (int i = 0; i < count; ++i) {
			Object ref = mReferences[i];
			if (ref instanceof Leak) {
				Leak leak = (Leak) ref;
				if (leak.mTotalSize == 0) {
					leak.computeTotalSize();
					mTotalSize += leak.mTotalSize;
				}
			}
		}
	}

	void clearTotalSize() {
		// first, clear our total size.
		mTotalSize = 0;
		
		// then, visit all nodes that haven't been visited,
		// and clear each one's total size.
		int count = mReferences.length;
		for (int i = 0; i < count; ++i) {
			Object ref = mReferences[i];
			if (ref instanceof Leak) {
				Leak leak = (Leak) ref;
				if (leak.mTotalSize != 0)
					leak.clearTotalSize();
			}
		}
	}

	public String toString() {
		return ("<A HREF=\"#" + mAddress + "\">" + mAddress + "</A> [" + mRefCount + "] " + mType + "{" + mTotalSize + "}");
	}
	
	static class ByCount implements QuickSort.Comparator {
		public int compare(Object obj1, Object obj2) {
			Leak l1 = (Leak) obj1, l2 = (Leak) obj2;
			return (l1.mRefCount - l2.mRefCount);
		}
	}

	/**
	 * Sorts in order of decreasing total size.
	 */
	static class ByTotalSize implements QuickSort.Comparator {
		public int compare(Object obj1, Object obj2) {
			Leak l1 = (Leak) obj1, l2 = (Leak) obj2;
			return (l2.mTotalSize - l1.mTotalSize);
		}
	}
}

public class leaksoup {
	public static void main(String[] args) {
		if (args.length == 0) {
			System.out.println("usage:  leaksoup [-blame] leaks");
			System.exit(1);
		}
		
		for (int i = 0; i < args.length; i++) {
			String arg = args[i];
			if (arg.charAt(0) == '-') {
				if (arg.equals("-blame"))
					FileLocator.USE_BLAME = true;
			} else {
				cook(arg);
			}
		}
	}
	
	static void cook(String inputName) {
		try {
			Vector vec = new Vector();
			Hashtable leakTable = new Hashtable();
			Hashtable types = new Hashtable();
			Histogram hist = new Histogram();
			BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(inputName)));
			String line = reader.readLine();
			while (line != null) {
				if (line.startsWith("0x")) {
					String addr = line.substring(0, 10);
					String name = line.substring(line.indexOf('<') + 1, line.indexOf('>'));
					int size;
					try {
						String str = line.substring(line.indexOf('(') + 1, line.indexOf(')')).trim();
						size = Integer.parseInt(str);
					} catch (NumberFormatException nfe) {
						size = 0;
					}

					// generate a unique type for this object.
					String key = name + "_" + size;
					Type type = (Type) types.get(key);
					if (type == null) {
						type = new Type(name, size);
						types.put(key, type);
					}
					
					// read in fields.
					vec.setSize(0);
					for (line = reader.readLine(); line != null && line.charAt(0) == '\t'; line = reader.readLine())
						vec.addElement(line.substring(1, 11));
					Object[] refs = new Object[vec.size()];
					vec.copyInto(refs);
					
					// read in stack crawl.
					vec.setSize(0);
					for (line = reader.readLine(); line != null && !line.equals("Leaked composite object at:"); line = reader.readLine())
						vec.addElement(line.intern());
					Object[] crawl = new Object[vec.size()];
					vec.copyInto(crawl);
					
					// record the leak.
					leakTable.put(addr, new Leak(addr, type, refs, crawl));

					// count the leak types in a histogram.
					hist.record(type);
				} else {
					line = reader.readLine();
				}
			}
			reader.close();
			
			Leak[] leaks = new Leak[leakTable.size()];
			int leakCount = 0;
			long totalSize = 0;

			Hashtable parentTable = new Hashtable();

			// now, we have a table full of leaked objects, lets derive reference counts, and build the graph.
			Enumeration e = leakTable.elements();
			while (e.hasMoreElements()) {
				Leak leak = (Leak) e.nextElement();
				Object[] refs = leak.mReferences;
				int count = refs.length;
				for (int i = 0; i < count; i++) {
					String addr = (String) refs[i];
					Leak ref = (Leak) leakTable.get(addr);
					if (ref != null) {
						// increase the ref count.
						ref.mRefCount++;
						// change string to ref itself.
						refs[i] = ref;
						// add leak to ref's parents vector.
						Vector parents = (Vector) parentTable.get(ref);
						if (parents == null) {
							parents = new Vector();
							parentTable.put(ref, parents);
						}
						parents.addElement(leak);
					}
				}
				leaks[leakCount++] = leak;
				totalSize += leak.mType.mSize;
			}

			// be nice to the GC.
			leakTable.clear();
			leakTable = null;
			
			// set the parents of each leak.
			e = parentTable.keys();
			while (e.hasMoreElements()) {
				Leak leak = (Leak) e.nextElement();
				Vector parents = (Vector) parentTable.get(leak);
				if (parents != null)
					leak.setParents(parents);
			}
			
			// be nice to the GC.
			parentTable.clear();
			parentTable = null;
			
			// store the leak report in inputName + ".html"
			PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(new FileOutputStream(inputName + ".html"))));

			Date now = new Date();
			out.println("<TITLE>Leaks as of " + now + "</TITLE>");

			// print leak summary.
			out.println("<H2>Leak Summary</H2>");
			out.println("total objects leaked = " + leakCount + "<BR>");
			out.println("total memory leaked  = " + totalSize + " bytes.<BR>");
			
			// sort the leaks by reference count. then compute each root leak's total size.
			QuickSort byCount = new QuickSort(new Leak.ByCount());
			byCount.sort(leaks);
			for (int i = 0; i < leakCount; ++i) {
				Leak leak = leaks[i];
				if (leak.mTotalSize == 0)
					leak.computeTotalSize();
			}
			
			// print the object histogram report.
			printHistogram(out, hist);
			
			// print the leak report.
			printLeaks(out, leaks);
			
			out.close();
			
			// quit the application.
			System.exit(0);
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
	
	/**
	 * Sorts the bins of a histogram by (count * typeSize) to show the
	 * most pressing leaks.
	 */
	static class HistComparator implements QuickSort.Comparator {
		Histogram hist;
		
		HistComparator(Histogram hist) {
			this.hist = hist;
		}
	
		public int compare(Object obj1, Object obj2) {
			Type t1 = (Type) obj1, t2 = (Type) obj2;
			return (hist.count(t1) * t1.mSize - hist.count(t2) * t2.mSize);
		}
	}

	static void printHistogram(PrintWriter out, Histogram hist) throws IOException {
		// sort the objects by histogram count.
		Object[] objects = hist.objects();
		QuickSort sorter = new QuickSort(new HistComparator(hist));
		sorter.sort(objects);
		
		out.println("<H2>Leak Histogram:</H2>");
		out.println("<PRE>");
		int count = objects.length;
		while (count > 0) {
			Object object = objects[--count];
			out.println(object.toString() + " : " + hist.count(object));
		}
		out.println("</PRE>");
	}

	static void printLeaks(PrintWriter out, Leak[] leaks) throws IOException {
		// sort the leaks by size.
		QuickSort bySize = new QuickSort(new Leak.ByTotalSize());
		bySize.sort(leaks);

		out.println("<H2>Leak Roots</H2>");
		
		out.println("<PRE>");

		int leakCount = leaks.length;
		for (int i = 0; i < leakCount; i++) {
			Leak leak = leaks[i];
			if (leak.mRefCount == 0)
				out.println(leak);
		}
		
		Type anchorType = null;

		// now, print the report, sorted by type size.
		for (int i = 0; i < leakCount; i++) {
			Leak leak = leaks[i];
			if (anchorType != leak.mType) {
				anchorType = leak.mType;
				out.println("\n<HR>");
				out.println("<A NAME=\"" + anchorType.mName + "_" + anchorType.mSize + "\"></A>");
				out.println("<H3>" + anchorType + " Leaks</H3>");
			}
			out.println("<A NAME=\"" + leak.mAddress + "\"></A>");
			if (leak.mParents != null) {
				out.print(leak);
				out.println(" <A HREF=\"#" + leak.mAddress + "_parents\">parents</A>");
			} else {
				out.println(leak);
			}
			// print object's fields:
			Object[] refs = leak.mReferences;
			int count = refs.length;
			for (int j = 0; j < count; j++)
				out.println("\t" + refs[j]);
			// print object's stack crawl:
			Object[] crawl = leak.mCrawl;
			count = crawl.length;
			for (int j = 0; j < count; j++) {
				String location = FileLocator.getFileLocation((String) crawl[j]);
				out.println(location);
			}
			// print object's parents.
			if (leak.mParents != null) {
				out.println("<A NAME=\"" + leak.mAddress + "_parents\"></A>");
				out.println("\nLeak Parents:");
				Leak[] parents = leak.mParents;
				count = parents.length;
				for (int j = 0; j < count; j++)
					out.println("\t" + parents[j]);
			}
		}

		out.println("</PRE>");
	}
}
